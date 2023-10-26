# Para compilar: ./compare_files.sh

#!/bin/bash

for ucfile in meta2/*.uc; do
    outfile="${ucfile%.uc}.out"
    resfile="${ucfile%.uc}.res"
    
    # Executar o uccompiler no arquivo .uc e gerar um arquivo .res
    ./uccompiler < "$ucfile" > "$resfile" #sem flag -l
    #./uccompiler -l < "$ucfile" > "$resfile" #com flag -l para printar lex
    #./uccompiler -t < "$ucfile" > "$resfile" #com flag -t para printar tree
    
    # Usar diff para comparar .res e .out
    if ! diff -q "$resfile" "$outfile" > /dev/null; then
        echo "Incompatibilidade encontrada em: $ucfile"
    fi
done
