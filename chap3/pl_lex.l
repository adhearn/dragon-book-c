%%

[ \t\n]+ { printf("%s", yytext); }
[aeiou][a-z]* { printf("%say", yytext); }
[b-df-hj-np-tv-z][a-z]* { printf("%s%cay", (yytext+1), *yytext); }

%%
