//Zugravu Anca-Daria, 452, tema1-compilatoare, Analizator Lexical
#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

// Token codes

#define INT_LIT 10

#define KEY_WORD 11

#define CHAR_STRING 12

#define IDENT 13

#define ADD_OP 21

#define SUB_OP 22

#define MULT_OP 23

#define DIV_OP 24

#define LEFT_PAREN 25

#define RIGHT_PAREN 26

#define RIGHT_CURLYBRACKET 27

#define LEFT_CURLYBRACKET 28

#define EQUAL_OP 29

#define SEMICOLON_OP 30

//Character types

#define LETTER 0

#define DIGIT 1

#define SYMBOL 2

#define QUOTES 3


class AnalizatorLexical{
  public:
    AnalizatorLexical();
    AnalizatorLexical(char *nume_fisier, char *nume_fisier_dest);

    int charType;
    char lexeme[100];
    char nextCh;
    int lexLength;
    int token;
    int nextToken;
    int getLexem();//functie pentru a prelua urmatorul lexem
    int searchSymbols(char ch);//functie pentru a cauta operatori, paranteze;returneaza TOKEN-ul lor
    vector<string> identificatori;//tabel in care pastram adresele indetificatorilor gasiti, pentru a-i putea compara cu cei noi gasiti
    void nextChar();
    void getChar();//functie care determina clasa din care face parte un caracter
    int analize();

  private:
    ifstream fisier_sursa;
    ofstream fisier_destinatie;
};

AnalizatorLexical::AnalizatorLexical()
{

}

AnalizatorLexical::AnalizatorLexical(char *nume_fisier, char *nume_fisier_dest)
{
    fisier_sursa.open(nume_fisier);
    fisier_destinatie.open(nume_fisier_dest);

    if(!fisier_destinatie)
    {
        cout<<"Nu s-a putut deschide fisierul "<<nume_fisier<<endl;
        exit(1);
    }

    if(!fisier_sursa)
    {
        cout<<"Nu s-a putut deschide fisierul "<<nume_fisier<<endl;
        exit(1);
    }
    else
    {

      getChar();

    do  {

        analize();

        } while (nextToken != EOF);

    }

    fisier_sursa.close();
    fisier_destinatie.close();
}

int AnalizatorLexical::searchSymbols(char ch)
{

    switch (ch)
    {
        case '(': nextChar(); nextToken = LEFT_PAREN;
            fisier_destinatie<<"\n Operator paranteza stanga ";
            break;
        case ')': nextChar(); nextToken = RIGHT_PAREN;
            fisier_destinatie<<"\n Operator paranteza dreapta ";
            break;
        case '{': nextChar(); nextToken = LEFT_CURLYBRACKET;
            fisier_destinatie<<"\n Acolada dreapta ";
            break;
        case '}': nextChar(); nextToken = RIGHT_CURLYBRACKET;
            fisier_destinatie<<"\n Acolada stanga ";
            break;
        case '+': nextChar(); nextToken = ADD_OP;
            fisier_destinatie<<"\n Operator plus ";
            break;
        case '-': nextChar(); nextToken = SUB_OP;
            fisier_destinatie<<"\n Operator minus ";
            break;
        case '*': nextChar(); nextToken = MULT_OP;
            fisier_destinatie<<"\n Operator mult ";
            break;
        case '/': nextChar(); nextToken = DIV_OP;
            fisier_destinatie<<"\n Operator div ";
            break;
        case '=': nextChar(); nextToken = EQUAL_OP;
            fisier_destinatie<<"\n Operator egal ";
            break;
        case ':': nextChar(); nextToken = SEMICOLON_OP;
            fisier_destinatie<<"\n Operator semicolon ";
            break;
        default: nextChar(); nextToken = EOF;
            fisier_destinatie<< "\n Eroare: Caracter necunoscut ";
            break;


    }
    return nextToken;
}

void AnalizatorLexical::nextChar()
{
    if(lexLength <= 100)
    {
        lexeme[lexLength++] = nextCh;
        lexeme[lexLength] = '\0';
    }
    else
        cout << "Eroare: Lexemul depaseste limita de caractere \n";
}

void AnalizatorLexical::getChar()
{
    fisier_sursa.get(nextCh);
    if(fisier_sursa.eof())
        nextCh = EOF;

    if(nextCh != EOF)
    {
        if(isalpha(nextCh))
            charType = LETTER;
        else{
            if(isdigit(nextCh))
                charType = DIGIT;
                else
                charType = SYMBOL;  }

      // if (nextCh == '"')
      //       charType = QUOTES;



    }
    else
        charType = EOF;

}

int AnalizatorLexical::analize()
{
    lexLength = 0;

    while(isspace(nextCh)) //ignoram spatiile
        getChar();

    switch(charType)
    {
        case LETTER: nextChar(); getChar();
            while(charType == LETTER || charType == DIGIT)
            {
                nextChar();
                getChar();
            }
            if(strcmp(lexeme,"for") == 0 || strcmp(lexeme,"if") == 0 || strcmp(lexeme,"int") == 0 || strcmp(lexeme,"while") == 0 || strcmp(lexeme,"do") == 0)
              {
                nextToken = KEY_WORD;
                fisier_destinatie << "\n CUVANT CHEIE ";}
              else
              {
                nextToken = IDENT;
                identificatori.push_back(lexeme);

                fisier_destinatie << "\n IDENTIFICATOR ";}
            break;

        case DIGIT: nextChar(); getChar();
            while(charType == DIGIT)
            {
                nextChar();
                getChar();
            }

            nextToken = INT_LIT;
            fisier_destinatie << "\n CONSTANTA NUMERICA ";
            break;
        // case QUOTES: nextChar(); getChar();
        //     do {
        //       // if(charType == '"')
        //       //   break;
        //       nextChar();
        //       getChar(); }while(charType != QUOTES);
        //       nextToken = CHAR_STRING;
        //       fisier_destinatie << "\n STRING DE CARACTERE ";
        //       break;

        case SYMBOL:
            searchSymbols(nextCh);
            getChar();
            break;

        case EOF:
            nextToken = EOF;
            break;
    }

    fisier_destinatie << "cu tokenul " <<nextToken<<" si valoarea "<<lexeme<<endl;


    return nextToken;
}


int main(int argc, char *argv[]){

  AnalizatorLexical analizatorLexical;
  ifstream fisier_sursa;
  ofstream fisier_destinatie;

    if(argc != 3)
    {
        cout<<"Utilizare program: "<<argv[0]<<" fisier_intrare fisier_iesire"<<endl;
        exit(1);
    }

    AnalizatorLexical lex(argv[1],argv[2]);

  return 0;
}
