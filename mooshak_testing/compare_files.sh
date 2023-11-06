# Para compilar: ./compare_files.sh

#!/bin/bash

Incompatibilidades=0

for ucfile in meta2/*.uc; do
    outfile="${ucfile%.uc}.out"
    resfile="${ucfile%.uc}.res"
    
     # Executar o uccompiler no arquivo .uc e gerar um arquivo .res
    ./uccompiler < "$ucfile" > "$resfile"
    
    # Usar diff para comparar .res e .out
    if ! diff -q "$resfile" "$outfile" > /dev/null; then
        echo "Incompatibilidade encontrada em: $ucfile"
        # Incrementa o contador de incompatibilidades
        ((incompatibilidades++))
    fi
done

# Imprime o total de incompatibilidades encontradas
echo "Total de incompatibilidades encontradas: $incompatibilidades"