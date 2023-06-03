#include <regex.h>
#include <stdio.h>
#include <sys/types.h>

int match(char *string, char *pattern, regex_t *re);
int main(int argc, char *argv[]) {
  regex_t regex;
  int reti;
  char msgbuf[100];
  int e = 0;

  /* Compile regular expression */
  reti = regcomp(&regex, "^a[[:alnum:]]", 0);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    e = 1;
  }

  /* Execute regular expression */
  if (e != 1) {
    reti = regexec(&regex, "abc", 0, NULL, 0);
    if (!reti) {
      puts("Match");
    } else if (reti == REG_NOMATCH) {
      puts("No match");
    } else {
      regerror(reti, &regex, msgbuf, sizeof(msgbuf));
      fprintf(stderr, "Regex match failed: %s\n", msgbuf);
      e = 1;
    }
  }
  /* Free compiled regular expression if you want to use the regex_t again */
  regfree(&regex);
}

int match(char *string, char *pattern, regex_t *re) {
  int status;

  if ((status = regcomp(re, pattern, REG_EXTENDED)) != 0) return (status);
  status = regexec(re, string, 0, NULL, 0);
  return (status);
}
