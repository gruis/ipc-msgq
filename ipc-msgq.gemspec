require File.expand_path("../lib/ipc/msgq/version", __FILE__)
require "rubygems"
::Gem::Specification.new do |s|
  s.name                        = "ipc-msgq"
  s.version                     = Ipc::MsgQ::VERSION
  s.platform                    = ::Gem::Platform::RUBY
  s.authors                     = ["caleb"]
  s.email                       = ["msgq@simulacre.org"]
  s.homepage                    = "https://github.com/simulacre/ipc-msgq"
  s.summary                     = "Provides SystemV Message Queues"
  s.description                 = "Ipc::MsgQ provides SystemV message queue interprocess communication"
  s.required_rubygems_version   = ">= 1.3.6"
  s.rubyforge_project           = "ipc-msgq"
  s.files                       = Dir["lib/**/*.rb", "bin/*", "*.md"]
  s.require_paths               = ['lib']
  s.executables               = Dir["bin/*"].map{|f| f.split("/")[-1] }

  # If you have C extensions, uncomment this line
  s.extensions = "ext/extconf.rb"

  # s.add_dependency "otherproject", "~> 1.2"
end
