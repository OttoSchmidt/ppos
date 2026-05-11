#!/bin/bash

RED="\e[0;31m"
YELLOW="\e[0;33m"
GREEN="\e[0;32m"
NOC="\e[0;37m"

DIR_HEADERS=("./kernel" "${HOME}/Downloads/ppos-aluno/kernel")

if [ ! -d "${DIR_HEADERS[0]}" ] || [ ! -d "${DIR_HEADERS[1]}" ]; then
	echo -e "${RED}diretorio(s) nao encontrado(s)!${NOC} diretorios esperados:"
	echo -e "\t- ${DIR_HEADERS[0]} -- ${YELLOW}projeto atual${NOC}\n\t- ${DIR_HEADERS[1]} -- ${YELLOW}projeto sem modificacoes${NOC}\n"
	exit 1
fi

cd ${DIR_HEADERS[0]}
HEADERS=$(ls *.h | sed 's/tcb.h//g' | xargs)
cd ..

echo -e "${YELLOW}verificando:${NOC} ${HEADERS}"

ALL_EQUAL=1
for HEADER in ${HEADERS}; do
	diff ${DIR_HEADERS[0]}/$HEADER ${DIR_HEADERS[1]}/$HEADER > diferenca.txt
	if [ $? -ne 0 ]; then
		ALL_EQUAL=0
		echo -e "\n${RED}- diferenca detectada no arquivo: ${HEADER}${NOC}"
		cat diferenca.txt
		echo -e "==================================="
	fi
done

rm diferenca.txt

if [ $ALL_EQUAL -eq 1 ]; then
	echo -e "\n${GREEN}todos os arquivos de cabecalho sao iguais!${NOC}"
fi