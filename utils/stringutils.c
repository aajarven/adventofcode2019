void strip_newline(char* string){
	while (*string != '\n' && *string != '\0') {
		string++;
	}
	*string = '\0';
}
