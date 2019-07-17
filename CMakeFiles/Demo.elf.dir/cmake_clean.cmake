file(REMOVE_RECURSE
  "Demo.elf.pdb"
  "Demo.elf"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM C)
  include(CMakeFiles/Demo.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
