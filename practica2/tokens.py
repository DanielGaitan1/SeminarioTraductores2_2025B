# tokens.py

# Definición de tipos de tokens según la tabla del profesor
IDENT       = 0
ENTERO      = 1
REAL        = 2
CADENA      = 3
TIPO        = 4   # int, float, void
OP_SUMA     = 5   # +, -
OP_MUL      = 6   # *, /
OP_REL      = 7   # <, <=, >, >=
OP_OR       = 8   # ||
OP_AND      = 9   # &&
OP_NOT      = 10  # !
OP_IG       = 11  # ==, !=
PYC         = 12  # ;
COMA        = 13
PARI        = 14  # (
PARD        = 15  # )
LLI         = 16  # {
LLD         = 17  # }
ASIG        = 18  # =
IF          = 19
WHILE       = 20
RETURN      = 21
ELSE        = 22
PESOS       = 23  # fin de entrada
