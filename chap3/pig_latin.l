%%

[ \t\n]+ { printf(" "); }
[aeiou][a-z]* { printf("%say", yytext); }
[^aeiou][a-z]* { printf("%scay", (yytext+1), *yytext); }

%%
