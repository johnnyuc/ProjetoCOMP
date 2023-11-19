#!/bin/bash

# Define ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color
totalIncompatibilidades=0

echo -e "\n"

for ((meta=1; meta<=3; meta++)); do
    incompatibilidades=0
    
    for ucfile in meta${meta}/*.{uc,c}; do
        if [[ "$ucfile" == *.uc ]]; then
            outfile="${ucfile%.uc}.out"
            resfile="${ucfile%.uc}.res"
        elif [[ "$ucfile" == *.c ]]; then
            outfile="${ucfile%.c}.out"
            resfile="${ucfile%.c}.res"
        fi
       
        # Executar o uccompiler no arquivo .uc e gerar um arquivo .res
        if [ "$meta" -eq 1 ]; then
            ./uccompiler -l < "$ucfile" > "$resfile"
        elif [ "$meta" -eq 2 ]; then
            ./uccompiler -t < "$ucfile" > "$resfile"
        elif [ "$meta" -eq 3 ]; then
            ./uccompiler -s < "$ucfile" > "$resfile"
        fi
        
        # Usar diff para comparar .res e .out
        if ! diff -q "$resfile" "$outfile" > /dev/null; then
            echo "Erro > $ucfile"
            # Incrementa o contador de incompatibilidades
            ((incompatibilidades++))
            ((totalIncompatibilidades++))
        fi
    done
    
    if [ "$incompatibilidades" -eq 0 ]; then
        echo -e "${YELLOW}Testes da meta${meta}: ${GREEN}OK${NC}"
    else
        echo -e "${YELLOW}Testes da meta${meta}: ${RED}NOK${NC}"
    fi
done


# Imprime o total de incompatibilidades encontradas
if [ "$totalIncompatibilidades" -eq 0 ]; then
    echo -e "${YELLOW}Todos os testes: ${GREEN}OK${NC}"
else
    echo "[Total incomp: ${totalIncompatibilidades}]"
fi
