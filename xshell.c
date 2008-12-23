
#include "roxml.h"

#define LTRIM(n)	while(((*n==' ')||(*n=='\t'))&&(*n!='\0')) { n++; }
#define RTRIM(n)	while((*n!=' ')&&(*n!='\t')&&(*n!='\r')&&(*n!='\n')&&(*n!='\0')) { n++; } *n = '\0';

int running = 0;

int main(int argc, char ** argv)
{
	node_t *root;
	node_t *cur;
	
	if(argc < 2)	{
		return -1;
	}
	root = roxml_load_doc(argv[1]);
	cur = root;
	
	running = 1;
	
	while(running)	{
		char *n;
		char *command;
		char cmd[512];
		fprintf(stdout, "[%s] > ",roxml_get_name(cur)); 
		cmd[0] = '\0';
		fgets(cmd, 511, stdin);

		n = cmd;
		LTRIM(n)
		command = n;
		if(strlen(cmd) == 0)	{
			fprintf(stdout,"\n");
			running = 0;
		} else if(strncmp(command, "ls", 2) == 0)	{
			int i, nb = roxml_get_son_nb(cur);
			fprintf(stdout,"%d elem in %s:\n\t", nb, roxml_get_name(cur));
			for(i = 0; i < nb; i++)	{
				node_t *n = roxml_get_son_nth(cur, i);
				char *name = roxml_get_name(n);
				if(roxml_get_son_nb(roxml_get_son_nth(cur, i)) > 0)	{
					fprintf(stdout,"%s/\t",name);
				} else	{
					fprintf(stdout,"%s\t",name);
				}
				free(name);
				if(i % 5 == 4)	{
					fprintf(stdout,"\n\t");
				}
			}
			fprintf(stdout,"\n");
		} else if(strncmp(command, "cd", 2) == 0)	{
			char* arg;
			int found = 0;
			n = command +2;
			LTRIM(n)
			arg = n;
			RTRIM(n)
			if(strcmp(arg, "..") == 0)	{
				found = 1;
				cur = roxml_get_parent(cur);
			} else	{
				int i, nb = roxml_get_son_nb(cur);
				for(i = 0; i < nb && !found; i++)	{
					if(strcmp(roxml_get_name(roxml_get_son_nth(cur, i)), arg)==0)	{
						found = 1;
						if(roxml_get_son_nb(roxml_get_son_nth(cur, i)) > 0)	{
							cur = roxml_get_son_nth(cur, i);
						} else	{
							fprintf(stdout,"This is a child node\n");
						}
					}
				}
			}
			if(!found)	{
				fprintf(stdout,"No such node\n");
			}
		} else if(strncmp(command, "ncd", 2) == 0)	{
			char* arg;
			int argn = 0;
			int found = 0;
			n = command +3;
			LTRIM(n)
			argn = atoi(n);
			RTRIM(n)
			n++;
			LTRIM(n)
			arg = n;
			RTRIM(n)
			int i, nb = roxml_get_son_nb(cur);
			for(i = 0; i < nb && !found; i++)	{
				if(strcmp(roxml_get_name(roxml_get_son_nth(cur, i)), arg)==0)	{
					argn--;
					if(argn == 0)	{
						found = 1;
						if(roxml_get_son_nb(roxml_get_son_nth(cur, i)) > 0)	{
							cur = roxml_get_son_nth(cur, i);
						} else	{
							fprintf(stdout,"This is a child node\n");
						}
					}
				}
			}
			if(!found)	{
				fprintf(stdout,"No such node\n");
			}
		} else if(strncmp(command, "cat", 3) == 0)	{
			int len;
			char* content;
			char* arg;
			int found = 0;
			n = command +3;
			LTRIM(n)
			arg = n;
			RTRIM(n)
			int i, nb = roxml_get_son_nb(cur);
			for(i = 0; i < nb && !found; i++)	{
				if(strcmp(roxml_get_name(roxml_get_son_nth(cur, i)), arg)==0)	{
					found = 1;
					if(roxml_get_son_nb(roxml_get_son_nth(cur, i)) > 0)	{
						fprintf(stdout,"This is not a child node\n");
					} else	{
						len = roxml_get_content(roxml_get_son_nth(cur, i), NULL);
						content = (char*)malloc(sizeof(char)*(len+1));
						roxml_get_content(roxml_get_son_nth(cur, i), content);
						fprintf(stdout,"%s\n",content);
						free(content);
					}
				}
			}
			if(!found)	{
				fprintf(stdout,"No such node\n");
			}
		} else if(strncmp(command, "ncat", 4) == 0)	{
			int len;
			char* content;
			char* arg;
			int argn;
			int found = 0;
			n = command +4;
			LTRIM(n)
			argn = atoi(n);
			RTRIM(n)
			n++;
			LTRIM(n)
			arg = n;
			RTRIM(n)
			int i, nb = roxml_get_son_nb(cur);
			for(i = 0; i < nb && !found; i++)	{
				if(strcmp(roxml_get_name(roxml_get_son_nth(cur, i)), arg)==0)	{
					argn--;
					if(argn == 0)	{
						found = 1;
						if(roxml_get_son_nb(roxml_get_son_nth(cur, i)) > 0)	{
							fprintf(stdout,"This is not a child node\n");
						} else	{
							len = roxml_get_content(roxml_get_son_nth(cur, i), NULL);
							content = (char*)malloc(sizeof(char)*(len+1));
							roxml_get_content(roxml_get_son_nth(cur, i), content);
							fprintf(stdout,"%s\n",content);
							free(content);
						}
					}
				}
			}
			if(!found)	{
				fprintf(stdout,"No such node\n");
			}
		} else if(strncmp(command, "file", 4) == 0)	{
			int nb, i, j;
			char* arg;
			int found = 0;
			n = command +4;
			LTRIM(n)
			arg = n;
			RTRIM(n)
			nb = roxml_get_son_nb(cur);
			for(i = 0; i < nb && !found; i++)	{
				if(strcmp(roxml_get_name(roxml_get_son_nth(cur, i)), arg)==0)	{
					found = 1;
					if(roxml_get_son_nb(roxml_get_son_nth(cur, i)) > 0)	{
						fprintf(stdout,"%s is a node with %d arguments:\n", roxml_get_name(roxml_get_son_nth(cur, i)),
							roxml_get_nb_attr(roxml_get_son_nth(cur, i)));
					} else	{
						fprintf(stdout,"%s is a child node with %d arguments:\n",roxml_get_name(roxml_get_son_nth(cur, i)),
							roxml_get_nb_attr(roxml_get_son_nth(cur, i)));
					}
					nb = roxml_get_nb_attr(roxml_get_son_nth(cur, i));
					for(j = 0; j < nb; j++) {
						fprintf(stdout,"\t%s=%s\n", roxml_get_attr_nth(roxml_get_son_nth(cur, i), j),
								roxml_get_attr_val_nth(roxml_get_son_nth(cur, i), j));
					}
				}
			}
			if(!found)	{
				fprintf(stdout,"No such node\n");
			}
		} else if(strncmp(command, "nfile", 5) == 0)	{
			int nb, i, j;
			char* arg;
			int argn;
			int found = 0;
			n = command +5;
			LTRIM(n)
			argn = atoi(n);
			RTRIM(n)
			n++;
			LTRIM(n)
			arg = n;
			RTRIM(n)
			nb = roxml_get_son_nb(cur);
			for(i = 0; i < nb && !found; i++)	{
				if(strcmp(roxml_get_name(roxml_get_son_nth(cur, i)), arg)==0)	{
					argn--;
					if(argn == 0)	{
						found = 1;
						if(roxml_get_son_nb(roxml_get_son_nth(cur, i)) > 0)	{
							fprintf(stdout,"%s is a node with %d arguments:\n", roxml_get_name(roxml_get_son_nth(cur, i)),
								roxml_get_nb_attr(roxml_get_son_nth(cur, i)));
						} else	{
							fprintf(stdout,"%s is a child node with %d arguments:\n",roxml_get_name(roxml_get_son_nth(cur, i)),
								roxml_get_nb_attr(roxml_get_son_nth(cur, i)));
						}
						nb = roxml_get_nb_attr(roxml_get_son_nth(cur, i));
						for(j = 0; j < nb; j++) {
							fprintf(stdout,"\t%s=%s\n", roxml_get_attr_nth(roxml_get_son_nth(cur, i), j),
									roxml_get_attr_val_nth(roxml_get_son_nth(cur, i), j));
						}
					}
				}
			}
			if(!found)	{
				fprintf(stdout,"No such node\n");
			}
		} else if(strncmp(command, "exit", 4) == 0)	{
			running = 0;
		} else if(strncmp(command, "quit", 4) == 0)	{
			running = 0;
		}
	}

	roxml_close_doc(root);
	return 0;
}

