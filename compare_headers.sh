#!/bin/bash

RED="\e[0;31m"
YELLOW="\e[0;33m"
GREEN="\e[0;32m"
NOC="\e[0;37m"

DIR_BASE=("./" "${HOME}/Downloads/ppos-aluno")
if [ ! -d "${DIR_BASE[0]}" ] || [ ! -d "${DIR_BASE[1]}" ]; then
	echo -e "${RED}diretorio(s) nao encontrado(s)!${NOC} diretorios esperados:"
	echo -e "\t- ${DIR_BASE[0]} -- ${YELLOW}projeto atual${NOC}\n\t- ${DIR_BASE[1]} -- ${YELLOW}projeto sem modificacoes${NOC}\n"
	exit 1
fi

# recuperar lista de arquivos que nao devem ser modificados
FILES_TO_COMPARE=$(grep -rl "ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO" | sed "s/compare_headers.sh//g" | xargs)

ALL_EQUAL=1
echo -e "${YELLOW}verificando:${NOC} ${FILES_TO_COMPARE}"

for FILE in ${FILES_TO_COMPARE}; do
	diff ${DIR_BASE[0]}/$FILE ${DIR_BASE[1]}/$FILE > diferenca.txt
	if [ $? -ne 0 ]; then
		ALL_EQUAL=0
		echo -e "\n${RED}- diferenca detectada no arquivo: ${FILE}${NOC}"
		cat diferenca.txt
		echo -e "==================================="
	fi
done

if [ $ALL_EQUAL -eq 1 ]; then
	echo -e "\n${GREEN}todos os arquivos sao iguais!${NOC}"
fi

rm diferenca.txt
