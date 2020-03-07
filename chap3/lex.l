%%

[ \t\n]+ { printf(yytext); }
"float" { printf("double"); }
[^ \t\n]+ { printf(yytext); }

%%
