MRuby::Gem::Specification.new('mruby-mrgss') do |spec|
  spec.license = 'Apache License Version 2.0 '
  spec.author  = 'Manuel Quinteros, Ramiro Rojo'

  # Add compile flags
  # spec.cc.flags << ''

  # Add cflags to all
  # spec.mruby.cc.flags << '-g'

  # Add libraries
  spec.linker.libraries << 'glew32'
  spec.linker.libraries << 'Ws2_32'
  spec.linker.libraries << 'pthread'
  spec.linker.libraries << 'SDL2'
  spec.linker.libraries << 'SDL2main'
  spec.linker.libraries << 'opengl32'
  

  # Default build files
  # spec.rbfiles = Dir.glob("#{dir}/mrblib/*.rb")
  # spec.objs = Dir.glob("#{dir}/src/*.{c,cpp,m,asm,S}").map { |f| objfile(f.relative_path_from(dir).pathmap("#{build_dir}/%X")) }
  # spec.test_rbfiles = Dir.glob("#{dir}/test/*.rb")
  # spec.test_objs = Dir.glob("#{dir}/test/*.{c,cpp,m,asm,S}").map { |f| objfile(f.relative_path_from(dir).pathmap("#{build_dir}/%X")) }
  # spec.test_preload = 'test/assert.rb'

  # Values accessible as TEST_ARGS inside test scripts
  # spec.test_args = {'tmp_dir' => Dir::tmpdir}
end
