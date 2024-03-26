%{
    #include "Errores.h"
    int contadorLineas = 1;
%}

%x COMENTARIOMULTIPLE
%x COMILLASSIMPLES
%x COMILLASDOBLES

ENTERO          {decinteger}|{bininteger}|{octinteger}|{hexinteger}
decinteger       {nonzerodigit}([_]?{digit})*|"0"+([_]?0)*
bininteger       "0"("b"|"B")([_]?{bindigit})+
octinteger       "0"("o"|"O")([_]?{octdigit})+
hexinteger       "0"("x"|"X")([_]?{hexdigit})+
nonzerodigit     [1-9]
digit            [0-9]
bindigit         [01]
octdigit         [0-7]
hexdigit         {digit}|[a-fA-F]



FLOAT            ({pointfloat}|{exponentfloat})
pointfloat       ({digitpart}{fraction}|{digitpart}"."|{fraction})
exponentfloat    ({digitpart}|{pointfloat}){exponent}
digitpart        {digit}(["_"]{digit})*
fraction         "."{digitpart}
exponent         ("e"|"E")["+"|"-"]{digitpart}

IDENTIFICADOR    [_a-zA-Z][_a-zA-Z0-9]*

COMENTARIOSIMPLE "#"[^\n]*

lexemasIndividuales [(|)|[|]|{|}|.|,|:|;|!|&||]

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

(ENTERO)        return(INTEGER);
(FLOAT)         return(FLOATNUMBER);
(IDENTIFICADOR) return(ID);

(MAYORIGUAL)    return(MAYOR_IGUAL);    
(MENORIGUAL)    return(MENOR_IGUAL);    
(DISTINTO)      return(DISTINTO);      
(COMPARACIONEQ) return(COMPARACION_IGUAL); 
(MASIGUAL)      return(MAS_IGUAL);      
(MENOSIGUAL)    return(MENOS_IGUAL);    
(PORIGUAL)      return(POR_IGUAL);      
(DIVIDIDOIGUAL) return(DIVIDIDO_IGUAL); 
(MODULOIGUAL)   return(MODULO_IGUAL);   
(EXPONENTE)     return(EXPONENTE);     

\n              contadorLineas++;

"\"\"\""     BEGIN(COMENTARIOMULTIPLE);
<COMENTARIOMULTIPLE>[^\"\n]*
<COMENTARIOMULTIPLE>"\""+[^\"\n]*
<COMENTARIOMULTIPLE>\n contadorLineas++;
<COMENTARIOMULTIPLE>"\"\"\"" BEGIN(INITIAL); return(3);
<COMENTARIOMULTIPLE><<EOF>> printf("Comentario multilinea no cerrado\n"); BEGIN (INITIAL);


"\'" yymore(); BEGIN(COMILLASSIMPLES);
<COMILLASSIMPLES>\n yymore(); contadorLineas++;
<COMILLASSIMPLES>"\'" BEGIN(INITIAL); return(STRING);
<COMILLASSIMPLES><<EOF>> BEGIN(INITIAL); printTipoError(ERROR_ANALIZADOR_LEXICO, "Se llego al final de larchivo pero no se cerro el string\n", contadorLineas); return(2); yyrestart(yyin);
<COMILLASSIMPLES>[.]* yymore();

"\"" yymore(); BEGIN(COMILLASDOBLES);
<COMILLASDOBLES>\n yymore(); contadorLineas++;
<COMILLASDOBLES>"\"" BEGIN(INITIAL); return(STRING);
<COMILLASDOBLES><<EOF>> BEGIN(INITIAL); printTipoError(ERROR_ANALIZADOR_LEXICO, "Se llego al final de larchivo pero no se cerro el string\n", contadorLineas); return(2); yyrestart(yyin);
<COMILLASDOBLES>[.]* yymore();

<<EOF>> return(2);

%%





