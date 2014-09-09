#include "answer02.h"
#include<stdio.h>

size_t my_strlen(const char * str) {
	int pos = -1;	
	while(str[++pos] != '\0') {}
	return pos;
}

int my_countchar(const char * str, char ch) {
	int n = 0;
	int pos = -1;
	while(str[++pos] != '\0') {
		if(str[pos] == ch) {
			n++;
		}
	}
	return n;
}

char * my_strchr(const char * str, int ch) {
	int pos = -1;
	int len = my_strlen(str);
	const char * match_idx = NULL;
		while(str[++pos] != '\0' && match_idx == NULL) {
			if(str[pos] == ch) {
				 match_idx = &str[pos];
			}
		}
		if (ch == '\0') {
			return (char *) str + len;
		}
		else {
			return (char *) match_idx;
		}
}

char * my_strrchr(const char * str, int ch) {
	int pos = my_strlen(str) - 1;
	int len = my_strlen(str);
	const char * match_idx = NULL;
	if (pos < 0) {
		pos = 0;
	}
	while(pos >= 0 && match_idx == NULL) {
		if(str[pos] == ch) {
			match_idx = &str[pos];
		}
		pos--;
		}
	if (ch == '\0') {
		return (char *) str + len;
	}  
	else {
		return (char *) match_idx;
	}
}



char * my_strstr(const char * haystack, const char * needle) {
	int posh = -1;
	int posn = 0;
	int count = 0;
	int n = 0;
	int hlength = my_strlen(haystack);
	int nlength = my_strlen(needle);
	while (++posh < hlength && count < nlength) {
		if (haystack[posh] == needle[posn]) {
			count++;
			posn++;
		}
		else {
			if (haystack[posh] == needle[0]) {
				count = 1;
				posn = 1;
			}
			else {
				count = 0;
				posn = 0;
			}
		}
		if  (nlength == count) {
			n = posh - count + 1;
		}
	}
	
	
	if (nlength == 0) {
		return (char *) haystack;
	}
	else if (nlength == count) {
		return (char *) haystack + n;
	}
	else {
		return NULL;
	}
}



char * my_strcpy(char * dest, const char * src) {
	int pos;
	int length = my_strlen(src);	
	for (pos = 0; pos <= length; pos++) {
		dest[pos] = src[pos];
	}
	return dest;
}

char * my_strcat(char * dest, const char * src) {
	int pos;
	int spos = 0;
	int dlength = my_strlen(dest);
    int length = my_strlen(src);
	for (pos = dlength; pos - dlength <= length; pos++) {
		dest[pos] = src[spos++];
	}   
	return dest;
}

int my_isspace(int ch) {
	if (ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v')
		return 1;
	else
		return 0;
}

int my_atoi(const char * str) {
	int pos = 0;
	int ret = 0;
	int sign = 1;
	while (my_isspace(str[pos])) {
		pos++;
	}
	if (str[pos] == '-') {
		sign = -1;
		pos++;
	}
	while (str[pos]=='0'||str[pos]=='1'||str[pos]=='2'||str[pos]=='3'||str[pos]=='4'||str[pos]=='5'||str[pos]=='6'||str[pos]=='7'||str[pos]=='8'||str[pos]=='9') {
		if (str[pos] == '0') {
			ret = ret*10 + 0;
		}
		else if (str[pos] == '1') {
			ret = ret*10 + 1;
		}
		else if (str[pos] == '2') {
			ret = ret*10 + 2;
		}
		else if (str[pos] == '3') {
			ret = ret*10 + 3;
		}
		else if (str[pos] == '4') {
			ret = ret*10 + 4;
		}
		else if (str[pos] == '5') {
			ret = ret*10 + 5;
		}
		else if (str[pos] == '6') {
			ret = ret*10 + 6;
		}
		else if (str[pos] == '7') {
			ret = ret*10 + 7;
		}
		else if (str[pos] == '8') {
			ret = ret*10 + 8;
		}
		else if (str[pos] == '9') {
			ret = ret*10 + 9;
		}
		pos++;
	}
	ret = ret * sign;
	return ret;
}
 
