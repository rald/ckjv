#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "common.h"

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"

#define INFO_IMPLEMENTATION
#include "info.h"

#define TOKEN_IMPLEMENTATION
#include "token.h"

#define LEXER_IMPLEMENTATION
#include "lexer.h"

#define PARSER_IMPLEMENTATION
#include "parser.h"

#define CITE_IMPLEMENTATION
#include "cite.h"

static Info **infos=NULL;
static size_t ninfos=0;

static Token **tokens=NULL;
static size_t ntokens=0;

static Cite **cites=NULL;
static size_t ncites=0;

void parsein(char *msg) {

	trim(msg);

	size_t page=0;
	char text[STRING_MAX];

	if(sscanf(msg,".kjv page %zu %[^\n]\n",&page,text)==2) {
		lex(&tokens,&ntokens,text);
		parse(infos,ninfos,tokens,ntokens,&cites,&ncites);
		Cites_Print(KJV_BOOK_PATH,page,infos,ninfos,cites,ncites);
		Tokens_Free(&tokens,&ntokens);
		Cites_Free(&cites,&ncites);
	} else if(sscanf(msg,".kjv %[^\n]\n",text)==1) {
		lex(&tokens,&ntokens,text);
		parse(infos,ninfos,tokens,ntokens,&cites,&ncites);
		Cites_Print(KJV_BOOK_PATH,1,infos,ninfos,cites,ncites);
		Tokens_Free(&tokens,&ntokens);
		Cites_Free(&cites,&ncites);
	} else if(sscanf(msg,".skjv page %zu %[^\n]\n",&page,text)==2) {
		search(KJV_BOOK_PATH,page,text);
	} else if(sscanf(msg,".skjv %[^\n]\n",text)==1) {
		search(KJV_BOOK_PATH,1,text);
	} else if(strcasecmp(msg,".pkjv")==0) {
	    pick(KJV_BOOK_PATH);
	}
}

int main(int argc,char *argv[]) {

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;

	srand(time(NULL));

	Info_Load(&infos,&ninfos,KJV_INFO_PATH);

	printf("> ");
	while((rlen=getline(&line,&llen,stdin))!=-1) {
		if(rlen>0) parsein(line);
		free(line);
		line=NULL;
		llen=0;
		rlen=0;
		printf("> ");
	}

	Infos_Free(&infos,&ninfos);

	return 0;
}
