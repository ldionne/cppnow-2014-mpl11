##############################################################################
# Mini library to manipulate compilers
##############################################################################
require 'benchmark'
require 'tempfile'


class CompilationError < RuntimeError
  def initialize(command_line, code, compiler_error_message)
    @cli = command_line
    @code = code
    @compiler_stderr = compiler_error_message
  end

  def to_s
    <<-EOS
compilation failed when invoking "#{@cli}"
compiler error message was:
#{'-' * 80}
#{@compiler_stderr}

full compiled file was:
#{'-' * 80}
#{@code}
EOS
  end
end

CompilationStatistics = Struct.new(:peak_memusg, :wall_time) do
  def to_s
<<-EOS
maximum resident set size: #{peak_memusg} kb
wall time:                 #{wall_time} s
EOS
  end
end

# This class contains the interface of compiler frontends.
#
# It also manages the registration of new compiler frontends. To register
# a new compiler frontend, simply create a new instance of `Compiler`
# implementing the required methods (they are documented).
class Compiler
  @@compilers = Hash.new

  # Compiler.list: Compilers
  #
  # Returns an array of the currently registered compilers.
  def self.list
    @@compilers.values
  end

  # Compiler[]: String or Symbol -> Compiler
  #
  # Returns the compiler associated to the given id.
  def self.[](id)
    if !@@compilers.has_key? id.to_sym
      raise ArgumentError, "unknown compiler #{id}"
    end
    @@compilers[id.to_sym]
  end

  # Compiler[]=: String or Symbol x Compiler -> Compiler
  #
  # Register a new compiler with the given id.
  def self.[]=(id, compiler)
    if @@compilers.has_key? id.to_sym
      raise ArgumentError,
            "overwriting existing compiler #{id} with #{compiler}"
    end
    @@compilers[id.to_sym] = compiler
  end

  # initialize: String or Symbol -> Compiler
  #
  # Create and register a new compiler with the given id.
  def initialize(id)
    @id = id.to_sym
    Compiler[@id] = self
  end

  # Maximum template recursion depth supported by the compiler.
  # This must be set explicitly.
  def template_depth
    @template_depth || (raise NotImplementedError)
  end
  attr_writer :template_depth

  # Maximum constexpr recursion depth supported by the compiler.
  # This must be set explicitly.
  def constexpr_depth
    @constexpr_depth || (raise NotImplementedError)
  end
  attr_writer :constexpr_depth

  # id: Symbol
  #
  # A symbol uniquely identifying a given compiler.
  #
  # In most cases, it should be something carrying the name and the
  # version of the compiler. For example, an identifier for Clang v3.5
  # could be clang35.
  attr_reader :id

  # Show the name and the version of the compiler.
  #
  # This must be implemented in subclasses.
  def to_s
    raise NotImplementedError
  end

  # compile_file: Path -> CompilationStatistics
  #
  # Compile the given file and return compilation statistics.
  #
  # Additional include paths may be specified with the `include`
  # named argument.
  #
  # A `CompilationError` is be raised if the compilation fails for
  # whatever reason. Either this method or `compile_code` must be
  # implemented in subclasses.
  def compile_file(file, include: [])
    raise ArgumentError, "invalid filename #{file}" unless File.file? file
    code = File.read(file)
    compile_code(code, include: include)
  end

  # compile_code: String -> CompilationStatistics
  #
  # Compile the given string and return compilation statistics.
  #
  # This method has the same behavior as `compile_file`, except the code
  # is given as-is instead of being in a file. Either this method or
  # `compile_file` must be implemented in subclasses.
  def compile_code(code, include: [])
    tmp = Tempfile.new(["", '.cpp'])
    tmp.write(code)
    tmp.close
    compile_file(tmp.path, include: include)
  ensure
    tmp.unlink
  end
end # class Compiler


# Helper for compilers with a GCC-like frontend.
#
# Basically, `compile_file` will return the stderr after running the compiler;
# subclasses then parse the compiler-specific output to retrieve time and
# memory usage statistics.
class GccFrontend < Compiler
  def initialize(id, binary)
    super id
    @binary = `which #{binary}`.strip
    raise "#{binary} not found" unless $?.success?
  end

  def compile_file(file, include: [])
    flags = "-std=c++1y -o /dev/null -fsyntax-only -ftime-report"
    includes = include.map(&'-I '.method(:+)).join(' ')
    cli = "#{@binary} #{flags} #{includes} -c #{file}"

    _, stderr, status = Open3.capture3("/usr/bin/time -l #{cli}")
    raise CompilationError.new(cli, File.read(file), stderr) unless status.success?
    return stderr
  end

  def to_s
    `#{@binary} --version`.lines.first.strip
  end
end

class Clang < GccFrontend
  def compile_file(*)
    stderr = super
    peak_memusg = stderr.match(/(\d+)\s+maximum/)[1].to_i
    wall_time = stderr.match(/.+Total/).to_s.split[-3].to_f
    return CompilationStatistics.new(peak_memusg, wall_time)
  end

  def template_depth;  256; end
  def constexpr_depth; 512; end
end

class GCC < GccFrontend
  def compile_file(*)
    stderr = super
    peak_memusg = stderr.match(/(\d+)\s+maximum/)[1].to_i
    wall_time = stderr.match(/TOTAL.+/).to_s.split[-3].to_f
    return CompilationStatistics.new(peak_memusg, wall_time)
  end

  def template_depth;  900; end
  def constexpr_depth; 512; end
end

Clang.new(:apple51, 'clang')
Clang.new(:clang34, 'clang++-3.4')
Clang.new(:clang35, 'clang++-3.5')
GCC.new(:gcc49, 'g++-4.9')
