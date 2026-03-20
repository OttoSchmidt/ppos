#!/bin/bash

MAX_TESTS=14

function get_tests_name() {
	local test_id=$1
	local tests_name=""

	case $test_id in
		1) tests_name="pingpong-task1 pingpong-task2 pingpong-task3" ;;
		2) tests_name="pingpong-dispatcher" ;;
		3) tests_name="pingpong-scheduler" ;;
		4) tests_name="pingpong-preempcao" ;;
		5) tests_name="pingpong-contab pingpong-contab-prio pingpong-contab-stress" ;;
		6) tests_name="pingpong-wait pingpong-wait-stress" ;;
		7) tests_name="pingpong-sleep" ;;
		8) tests_name="pingpong-semaphore pingpong-semaphore-stress" ;;
		9) tests_name="pingpong-prodcons" ;;
		10) tests_name="pingpong-mqueue" ;;
		11) tests_name="pingpong-memoria pingpong-mqueue" ;;
		12) tests_name="pingpong-disco pingpong-disco-stress" ;;
		13) tests_name="pingpong-disco-stress" ;;
		14) tests_name="pingpong-cache-on pingpong-cache-off" ;;
		*) tests_name="-" ;;
	esac

	echo "$tests_name"
}

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

PROJECT_ID=$1
if [ -z "$PROJECT_ID" ]; then
	echo -e "${RED}Erro:${NC} Nenhum ID de projeto fornecido."
	echo -e "Uso: $0 <PROJECT_ID>"
	exit 1
fi

# extrair o número do projeto do argumento
PROJECT_NUMBER=$(tr -d -c 0-9 <<< "$PROJECT_ID")
if [ $PROJECT_NUMBER -lt 1 ] || [ $PROJECT_NUMBER -gt $MAX_TESTS ]; then
	echo -e "${RED}Erro:${NC} Número do projeto inválido. Deve ser entre 1 e $MAX_TESTS."
	exit 1
fi

for i in $(seq 1 $PROJECT_NUMBER); do
	echo -e "${BLUE}Executando testes do P$i...${NC}"

	# manter somente erros no console
	make p$i > /dev/null
	if [ $? -ne 0 ]; then
		echo -e "${RED}Erro:${NC} Compilacao dos testes do P$i falharam."
		exit 1
	fi

	ALL_TESTS_PASSED=true
	TESTS=$(get_tests_name $i)

	for test in $TESTS; do
		echo "Executando teste: $test"

		# executar o teste e redirecionar tudo para um arquivo
		./$test > ${test}-output.txt 2>&1
		if [ $? -ne 0 ]; then
			echo -e "${RED}Erro:${NC} Teste $test falhou. Verifique ${test}-output.txt para detalhes."
			ALL_TESTS_PASSED=false
		fi

		# comparar a saída do teste com a saída esperada
		diff ${test}-output.txt test/${test}.txt > ${test}-diff.txt
		if [ $? -ne 0 ]; then
			echo -e "${RED}Erro:${NC} Teste $test falhou. Saída difere do esperado. Verifique ${test}-diff.txt para detalhes."
			ALL_TESTS_PASSED=false
		fi
	done

	if $ALL_TESTS_PASSED; then
		echo -e "${GREEN}Sucesso:${NC} Todos os testes do P$i passaram!"
		rm *-output.txt *-diff.txt # limpar arquivos de saída e diffs
	else
		echo -e "${RED}Erro:${NC} Alguns testes do P$i falharam. Verifique os arquivos de saída para detalhes."
	fi
	echo -e "------------------------------\n"
done