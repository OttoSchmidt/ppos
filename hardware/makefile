# PingPongOS - PingPong Operating System
# Prof. Carlos A. Maziero, DINF UFPR
# Versão 2.0 -- Junho de 2025

# ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
# ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

# flags são importados do makefile superior

HDR = $(wildcard *.h)			# headers
SRC = $(wildcard *.c)			# fontes C
ASM = $(wildcard *.s)			# fontes assembly
OBJ = $(patsubst %.c,%.o,$(SRC))	# objetos .o
OBJ+= $(patsubst %.s,%.o,$(ASM))	# idem

# arquivo com a imagem do disco virtual
DISK = disk.dat

# regra default: reconstruir tudo
all   : $(OBJ)
$(OBJ): $(SRC) $(HDR)

# testa o conteudo do disco
test:
	@echo "Comparando imagens do disco:"
	-@diff -qs $(DISK) $(DISK)-inicial || true
	-@diff -qs $(DISK) $(DISK)-final   || true

# reinicia o conteúdo do disco
reset:
	-cp -f $(DISK)-inicial $(DISK)
	@echo "Conteúdo do disco reiniciado"
