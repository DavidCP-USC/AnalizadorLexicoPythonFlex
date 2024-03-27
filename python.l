%{
    #include "Errores.h"
    int contadorLineas = 1;
    #ifndef yywrap
    static int yywrap (void) { return 1; }
    #endif
%}

%x COMENTARIOMULTIPLE
%x COMILLASSIMPLES 
%x COMILLASDOBLES

ENTERO           {decinteger}|{bininteger}|{octinteger}|{hexinteger}
decinteger       {nonzerodigit}([_]?{digit})*|0+([_]?0)*
bininteger       0(b|B)([_]?{bindigit})+
octinteger       0(o|O)([_]?{octdigit})+
hexinteger       0(x|X)([_]?{hexdigit})+
digit            [0-9]
nonzerodigit     [1-9]
bindigit         [01]
octdigit         [0-7]
hexdigit         [0-9a-fA-F]

FLOAT            {pointfloat}|{exponentfloat}
pointfloat       {digitpart}?{fraction}|{digitpart}\.
exponentfloat    ({digitpart}|{pointfloat}){exponent}
digitpart        [0-9][0-9]*
fraction         \.{digitpart}
exponent         [eE][+-]?{digitpart}



IDENTIFICADOR    [_a-zA-Z][_a-zA-Z0-9]*

COMENTARIOSIMPLE "#"[^\n]*

LEXEMASINDIVIDUALES [\<|\>|\=|\/|\*|\-|\+|\(|\)|\[|\]|\{|\}|\.|\,|\:|\;|\!|\&|\|]

MAYORIGUAL      ">="
MENORIGUAL      "<="
DISTINTO        "!="
COMPARACIONEQ   "=="
MASIGUAL        "+="
MENOSIGUAL      "-="
PORIGUAL        "*="
DIVIDIDOIGUAL   "/="
MODULOIGUAL     "%="
EXPONENTE       "**"

%%

{ENTERO}        return(INTEGER);
{FLOAT}         return(FLOATNUMBER);
{IDENTIFICADOR} return(3);
{LEXEMASINDIVIDUALES}   return((int) yytext[0]);

{MAYORIGUAL}    return(MAYOR_IGUAL);    
{MENORIGUAL}    return(MENOR_IGUAL);    
{DISTINTO}      return(DISTINTO);      
{COMPARACIONEQ} return(COMPARACION_IGUAL); 
{MASIGUAL}      return(MAS_IGUAL);      
{MENOSIGUAL}    return(MENOS_IGUAL);    
{PORIGUAL}      return(POR_IGUAL);      
{DIVIDIDOIGUAL} return(DIVIDIDO_IGUAL); 
{MODULOIGUAL}   return(MODULO_IGUAL);   
{EXPONENTE}     return(EXPONENTE);   

{COMENTARIOSIMPLE}
\n              contadorLineas++;
[ \t]

"\"\"\""     BEGIN(COMENTARIOMULTIPLE);
<COMENTARIOMULTIPLE>[^\"\n]*
<COMENTARIOMULTIPLE>"\""[^\"\n]*
<COMENTARIOMULTIPLE>\n contadorLineas++;
<COMENTARIOMULTIPLE>"\"\"\"" BEGIN(INITIAL);
<COMENTARIOMULTIPLE><<EOF>> printf("Comentario multilinea no cerrado\n"); BEGIN (INITIAL);


"\'" yymore(); BEGIN(COMILLASSIMPLES);
<COMILLASSIMPLES>[^\'\n]* yymore();
<COMILLASSIMPLES>\n contadorLineas++; yymore();
<COMILLASSIMPLES>"\'" BEGIN(INITIAL); return(STRING);
<COMILLASSIMPLES><<EOF>> BEGIN(INITIAL); printTipoError(ERROR_ANALIZADOR_LEXICO, "Se llego al final de larchivo pero no se cerro el string\n", contadorLineas); return(2); yyrestart(yyin);

"\"" yymore(); BEGIN(COMILLASDOBLES);
<COMILLASDOBLES>[^\"\n]* yymore();
<COMILLASDOBLES>\n contadorLineas++; yymore();
<COMILLASDOBLES>"\"" BEGIN(INITIAL); return(STRING);
<COMILLASDOBLES><<EOF>> BEGIN(INITIAL); printTipoError(ERROR_ANALIZADOR_LEXICO, "Se llego al final de larchivo pero no se cerro el string\n", contadorLineas); return(2); yyrestart(yyin);

<<EOF>> return(2);

%%