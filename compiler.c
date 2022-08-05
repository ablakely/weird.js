/*
 * weird.js - Javascript obfuscation compiler
 *
 * Written by Aaron Blakely <aaron@ephasic.org>
 * Copyright (C) 2022 Ephasic Software (https://ephasic.org/wtf.js)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUMS 5

int charCount = 0;
int stringCount = 0;

struct charStruct
{
  char c;
  char *code;
  int size;
} chars[22];

struct stringsStruct
{
  char *str;
  char *code;
  int size;
} strings[4];

typedef struct numberArgs
{
  int len;
  int slen;
  int numbers[MAX_NUMS];
  char *strings[MAX_NUMS];
} numberArgs;

void strlcpy(char *to, const char *from, int len)
{
    memccpy(to, from, '\0', len);
}

int charIndex(char c)
{
  for (int i = 0; i < charCount; i++)
  {
    if (chars[i].c == c)
      return i;
  }

  return 0;
}

int stringIndex(char *str)
{
  for (int i = 0; i < stringCount; i++)
  {
    if (strcmp(str, strings[i].str) == 0)
      return i;
  }

  return -1;
}

int numberStrSize(int n)
{
  if (n > 1)
  {
    return 8*n-3;
  }
  else
  {
    return 5*n;
  }
}

char *number(int n)
{
  char *ret = calloc(n, sizeof(char)*8);
  char *one = "+!![]";

  for (int i = 0; i < n; i++)
  {
    strcat(ret, one);

    if (i+1 != n)
    {
      strcat(ret, " + ");
    }
  }

  return ret;
}

void initChar(char c, char *str, numberArgs args)
{
  int codelen= 0, i = 0, j = 0, slen = 0, argcnt = 0, sargcnt = 0, bufsize = 0, stridx = 0;
  char *buf;

  for (i = 0; i < args.len; i++)
  {
    bufsize += numberStrSize(args.numbers[i])+1;
  }

  if (args.slen > 0)
  {
    for (i = 0; i < args.slen; i++)
    {
      stridx = stringIndex(args.strings[i]);
      

      if (strings[stridx].code)
        bufsize += (int)strlen(strings[stridx].code)+10;

      if (i+1 != args.slen)
      {
        bufsize += 3;
      }
    }
  }

  codelen = (int)strlen(str);
  chars[charCount].c = c;

  buf = calloc(bufsize+50, sizeof(char));
  i = 0;

  while (str[i] != '\0')
  {
    if (str[i] == '?')
    {
      sprintf(buf, "%s%s", buf, number(args.numbers[argcnt]));
      j += (int)strlen(number(args.numbers[argcnt]));
      buf[j] = ' ';

      argcnt++;
      i++;
    }
    else if (str[i] == '@')
    {
      stridx = stringIndex(args.strings[sargcnt]);
      if (stridx != -1)
      {
        sprintf(buf, "%s%s", buf, strings[stridx].code);
        j += (int)strlen(strings[stridx].code);
      }
      else
      {
        strcat(buf, args.strings[sargcnt]);
        j += (int)strlen(args.strings[sargcnt])+1;
      }
      
      i++;
      sargcnt++;
    }
    else
    {
      buf[j] = str[i];
      i++;
      j++;
    }
  }


  chars[charCount].code = calloc((int)strlen(buf)+1, sizeof(char));
  strlcpy(chars[charCount].code, buf, (int)strlen(buf)+1);
  chars[charCount].size = (int)strlen(buf)+1;
  charCount++;

  free(buf);
}

void initString(char *str)
{
  int i, idx, slen, bufsize = 0;
  char *buf;
  
  slen = (int)strlen(str);

  for (i = 0; i < (int)strlen(str)+1; i++)
  {
    idx = charIndex(str[i]);
    bufsize += (int)strlen(chars[idx].code)+1;

    if (i+1 != slen)
    {
      bufsize += 3;
    }
  }

  buf = calloc(bufsize+2, sizeof(char));

  for (i = 0; i < slen; i++)
  {
    idx = charIndex(str[i]);
    sprintf(buf, "%s%s", buf, chars[idx].code);

    if (i+1 != slen)
    {
      strcat(buf, " + ");
    }
  }

  strings[stringCount].str = calloc((int)strlen(str)+1, sizeof(char));
  strlcpy(strings[stringCount].str, str, (int)strlen(buf)+1);
  strings[stringCount].code = calloc((int)strlen(buf)+1, sizeof(char));
  strlcpy(strings[stringCount].code, buf, (int)strlen(buf)+1);
  strings[stringCount].size = (int)strlen(buf)+1;
  stringCount++;

  free(buf);
}

void init_characters()
{
  numberArgs tmp;
  
  for (int i = 0; i < MAX_NUMS; i++)
  {
    tmp.strings[i] = calloc(35, sizeof(char));
  }

  tmp.slen = 0;
  tmp.len = 0;
  initChar('f', "(!{}+[])[+[]]", tmp);

  tmp.len = 1;

  tmp.numbers[0] = 7;
  initChar(' ', "({}+[])[?]", tmp);
 
  tmp.numbers[0] = 1;
  initChar('a', "(+{}+[])[?]", tmp);
  initChar('o', "({}+[])[?]", tmp);
  initChar('r', "(!![]+[])[?]", tmp);
  initChar('\\', "(/\\\\/+[])[?]", tmp);

  tmp.numbers[0] = 2;
  initChar('b', "({}+[])[?]", tmp);
  initChar('u', "(!![]+[])[?]", tmp);

  tmp.numbers[0] = 3;
  initChar('i', "((+!![]/+[])+[])[?]", tmp);
  initChar('s', "(![]+[])[?]", tmp);

  tmp.numbers[0] = 4;
  initChar('e', "({}+[])[?]", tmp);
  initChar('n', "((+!![]/+[])+[])[?]", tmp);

  tmp.numbers[0] = 5;
  initChar('c', "({}+[])[?]", tmp);

  tmp.numbers[0] = 6;
  initChar('t', "({}+[])[?]", tmp);

  initString("constructor");

  tmp.len = 1;
  tmp.slen = 1;
  tmp.numbers[0] = 9;
  tmp.strings[0] = "constructor";
  initChar('S', "[]+([]+(''[@]))[?]", tmp);

  tmp.numbers[0] = 14;
  initChar('g', "[]+([]+(''[@]))[?]", tmp);
  initChar('p', "[]+([]+(/-/[@]))[?]", tmp);

  initString("toString");

  tmp.len = 2;
  tmp.slen = 1;
  tmp.numbers[0] = 13;
  tmp.numbers[1] = 32;
  tmp.strings[0] = "toString";
  initChar('d', "[]+([]+(?)[@])[?]", tmp);

  tmp.numbers[0] = 17;
  tmp.numbers[1] = 18;
  initChar('h', "(?)[@](?)", tmp);

  tmp.numbers[0] = 22;
  tmp.numbers[1] = 23;
  initChar('m', "(?)[@](?)", tmp);

  initString("return escape");

  tmp.len = 1;
  tmp.slen = 2;
  tmp.strings[0] = "constructor";
  tmp.strings[1] = "return escape";
  tmp.numbers[0] = 2;
  initChar('C', "(()=>{})[@](@)()((/\\\\/+[])[+!![]])[?]", tmp);

  initString("fromCharCode");
}


char *charCode(char c)
{
  int i, idx, charc;
  numberArgs tmp;
  char *code;

  for (i = 0; i < charCount; i++)
  {
    if (chars[i].c == c)
      return chars[i].code;
  }

  tmp.len        = 1;
  tmp.slen       = 2;
  tmp.numbers[0] = (int) c;
  tmp.strings[0] = "constructor";
  tmp.strings[1] = "fromCharCode";

  initChar('?', "([]+[])[@][@](?)", tmp);

  idx = charIndex('?');
  code = chars[idx].code;

  charCount--;
  return code;
}

char *fromString(char *str)
{
  int i, idx, bufsize = 0;
  char *buf;

  for (i = 0; i < (int)strlen(str)+1; i++)
  {
    bufsize += strlen(charCode(str[i]))+4;
  }

  buf = calloc(bufsize, sizeof(char));

  for (i = 0; i < strlen(str)+1; i++)
  {
    strcat(buf, charCode(str[i]));

    if (i+1 != (int)strlen(str)+1)
    {
      strcat(buf, " + ");
    }
  }

  return buf;
}

char *compile(char *code, int evalWrapMode)
{
  int idx, bufsize = 16;
  char *compiled;

  idx = stringIndex("constructor");
  bufsize += strings[idx].size;
  bufsize += strlen(fromString(code));

  compiled = calloc(bufsize, sizeof(char));

  if (evalWrapMode == 1)
  {
    sprintf(compiled, "(()=>{})[%s](%s)()", strings[idx].code, fromString(code));
  }
  else
  {
    compiled = fromString(code);
  }

  return compiled;
}

void usage()
{
  printf("usage: weird [OPTIONS]... [IN File] [OUT FILE]\n");
  printf("JavaScript obfuscation compiler\n\n");
  printf("    -m - Disables wrapping final code in an eval() statement. (Used for compiling node.js modules)\n");
  printf("\nweird.js - Written by Aaron Blakely\nCopyright (C) 2022 Ephasic Software\n");
  printf("  https://github.com/ablakely/weird.js\n");
  exit(-1);
}

int getFileSize(char *file)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  int bufsize;

  fp = fopen(file, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1)
  {
    bufsize += strlen(line)+1;
  }

  return bufsize;
}

void compileFile(char *in, char *out, int evalWrapMode)
{
  FILE *fp, *wfp;
  char *line = NULL;
  char *compiled = NULL;
  char *buf = NULL;
  size_t len = 0;
  ssize_t read;
  int i, idx, bufsize, commentmode = 0;

  fp = fopen(in, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  wfp = fopen(out, "w");
  if (wfp == NULL)
    exit(EXIT_FAILURE);

  bufsize = getFileSize(in);
  buf = calloc(bufsize, sizeof(char));

  while ((read = getline(&line, &len, fp)) != -1)
  {
    for (i = 0; i < strlen(line)+1; i++)
    {
      // Ignore shebangs 
      if (line[i] == '#' && line[i+1] == '!')
      {
        while (line[i] != '\n')
          i++;

        continue;
      }

      // Ignore signle line comments
      if (line[i] == '/' && line[i+1] == '/')
      {
        while (line[i] != '\n')
          i++;

        continue;
      }

      // Ignore multiline comments
      else if (line[i] == '/' && line[i+1] == '*')
      {
        commentmode = 1;

        i++;
        continue;
      }
      else if (line[i] == '*' && line[i+1] == '/')
      {
        commentmode = 0;

        i++;
        continue;
      }
      else
      {
        if (line[i] != '\n')
        {
          if (commentmode != 1)
            sprintf(buf, "%s%c", buf, line[i]);
        }
        else
        {
          if (commentmode != 1)
            sprintf(buf, "%s ", buf);
        }
      }
    }
  }

  fputs(compile(buf, evalWrapMode), wfp);

  fclose(fp);
  fclose(wfp);

  if (line)
    free(line);

  if (compiled)
    free(compiled);

  free(buf);
}

int main(int argc, char *argv[])
{
  int evalWrapMode = 1;
  int argoffset = 0;
  init_characters();

  if (argc == 3)
  {
    compileFile(argv[1], argv[2], evalWrapMode);
  }
  else if (argc > 3)
  {
    for (int i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "-m") == 0)
      {
        evalWrapMode = 0;
        argoffset++;
      }
    }

    compileFile(argv[argoffset+1], argv[argoffset+2], evalWrapMode);
  }
  else
  {
    usage();
  }

  return 0;
}
