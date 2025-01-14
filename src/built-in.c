#include "../include/minishell.h"


void print_exported_environ(void)
{
    extern char **environ;
    for (int i = 0; environ[i]; i++)
    {
        // We print it in "declare -x KEY=VALUE" style (bash-like).
        // Optional: parse out KEY and VALUE to insert quotes around VALUE.
        // For a simpler approach, just do:
        printf("declare -x %s\n", environ[i]);
    }
}

void set_env_variable(const char *key, const char *value)
{
    extern char **environ;
    int   key_len = ft_strlen(key);

    // 1) Search existing environment for the key
    for (int i = 0; environ[i]; i++)
    {
        // Look for 'KEY=' at start of environ[i]
        // i.e. if strncmp matches and environ[i][key_len] == '='
        if (ft_strncmp(environ[i], key, key_len) == 0 && environ[i][key_len] == '=')
        {
            // 2) Found => replace old "KEY=old_value" with "KEY=new_value"
            free(environ[i]); // free the old "KEY=VALUE" string
            environ[i] = create_env_string(key, value);
            return;
        }
    }

    // 3) Not found => add new environment variable
    add_env_variable(key, value);
}

char *create_env_string(const char *key, const char *value)
{
    int   key_len = ft_strlen(key);
    int   val_len = ft_strlen(value);
    char  *env_string;

    // +2 => 1 for '=' and 1 for the trailing '\0'
    env_string = (char *)malloc(key_len + val_len + 2);
    if (!env_string)
        return (NULL); // handle error case as needed

    // copy key with ft_strlcpy
    ft_strlcpy(env_string, key, key_len + 1);
    env_string[key_len] = '=';
    // copy value right after '='
    ft_strlcpy(env_string + key_len + 1, value, val_len + 1);

    return (env_string);
}


void add_env_variable(const char *key, const char *value)
{
    extern char **environ;

    // 1) Count how many entries in environ
    int i = 0;
    while (environ[i])
        i++;

    // 2) Allocate new array (old_count + 2),
    //    because we need one extra for the new var and one for NULL terminator.
    char **new_env = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_env)
    {
        // handle error, e.g. perror or similar
        return;
    }

    // 3) Copy old environ pointers
    for (int j = 0; j < i; j++)
        new_env[j] = environ[j];

    // 4) Create "KEY=VALUE" and append
    new_env[i] = create_env_string(key, value);
    // 5) Null-terminate
    new_env[i + 1] = NULL;

    // Optionally, free the old array if you're sure you allocated it
    // in your own code.  Depending on your project specs, you might do:
    // free(environ);
    // but in some setups, 'environ' might be statically allocated by the system.
    // In 42 projects, typically we've duplicated environ at startup,
    // so we can safely free it. Adjust to your code:
    free(environ);

    // 6) Point `environ` to the new array
    environ = new_env;
}


void    handle_export(char **argv)
{
    extern char **environ;

    // If "export" has no arguments, just display the environment
    if (!argv[1])
    {
        print_exported_environ();
        return;
    }

    // Process each argument after 'export'
    for (int i = 1; argv[i]; i++)
    {
        // 1) Look for '=' in the argument
        char *equal_sign = ft_strchr(argv[i], '=');

        // 2) If found, separate KEY and VALUE
        if (equal_sign)
        {
            int key_len = equal_sign - argv[i]; // distance from argv[i] start to '='
            // Extract key
            char *key = ft_substr(argv[i], 0, key_len);
            // Extract value (after '=')
            char *value = ft_substr(argv[i], key_len + 1,
                                    ft_strlen(argv[i]) - (key_len + 1));

            // Validate key, etc. (optional: handle errors for invalid keys)

            // Update or add this KEY=VALUE in environ
            set_env_variable(key, value);

            free(key);
            free(value);
        }
        else
        {
            // No '=' => "export KEY"
            // By default, let's set KEY to empty if it doesn't exist
            // or do nothing if it already exists
            set_env_variable(argv[i], "");
        }
    }
}

void handle_exit(char **argv)
{
    int status = 0;
    if (argv[1])
        status = atoi(argv[1]);
    exit(status);
}
// void handle_cd(char **argv)
// {
// 	if (!argv[1]) 
// 	{
//         char *home = getenv("HOME");
//         if (home) 
// 		{
//             if (chdir(home) != 0)
//                 perror("cd"); // Print error if chdir fails
//         } 
// 		else
//             fprintf(stderr, "cd: HOME environment variable is not set\n");
//         return;
//     }
// 	if (chdir(argv[1]) != 0)
// 		printf("cd: no such file or directory: %s\n", argv[1]); // will come back to it to change to stderror
// }

void handle_cd(char **argv)
{
    if (!argv[1]) 
    {
        char *home = getenv("HOME");
        if (home) 
        {
            if (chdir(home) == 0)
            {
                // Update PWD in the environment
                char *cwd = getcwd(NULL, 0);
                if (cwd)
                {
                    setenv("PWD", cwd, 1);
                    free(cwd);
                }
            }
            else
                perror("cd"); // Print error if chdir fails
        } 
        else
            fprintf(stderr, "cd: HOME environment variable is not set\n");
        return;
    }
    if (chdir(argv[1]) == 0)
    {
        // Update PWD in the environment
        char *cwd = getcwd(NULL, 0);
        if (cwd)
        {
            setenv("PWD", cwd, 1);
            free(cwd);
        }
    }
    else
        fprintf(stderr, "cd: no such file or directory: %s\n", argv[1]);
}



void handle_unset(char **argv)
{
	(void)argv;
	extern char **environ;
	if (!argv[1])
	{
		const char *error_msg = "unset: Missing argument\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		return;
	}
	char **env = environ;
	while (*env)
	{
		if (strncmp(*env, argv[1], ft_strlen(argv[1])) == 0 && (*env)[strlen(argv[1])] == '=')
		{
			char **next = env;
			while (*next)
            {
                *next = *(next + 1);
                next++;
            }
            break;
        }
        env++;
		}
}

void handle_env(char **argv)
{
	(void)argv;
	extern char **environ;
	char **env = environ;

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}
void handle_pwd(char **argv)
{
	(void)argv;
	char *pwd = getenv("PWD");
    if (pwd)
        printf("%s\n", pwd);
    else
	{
        const char *error_msg = "pwd: Unable to retrieve current directory\n";
    	write(STDERR_FILENO, error_msg, strlen(error_msg));
	}
}

void handle_echo(char **argv)
{
	int i = 1; //start from 1 bec. 0 is the command itself
	int newline = 1; //default is to print newline
	int enable_escape = 0; //default is to disable escape

	// 1) parse flags
	while (argv[i] && argv[i][0] == '-' && argv[i][1] != '\0')
	{
		int j = 1;
		int valid_flag = 1; //default is to assume the flag is valid
		while (argv[i][j])
		{
			if (argv[i][j] == 'n')
				newline = 0;
			else if (argv[i][j] == 'e')
				enable_escape = 1;
			else if (argv[i][j] == 'E')
				enable_escape = 0;
			else
			{
				valid_flag = 0;
				break;
			}
			j++;
		}
		if (!valid_flag)
			break;
		i++;
	}

	// 2) print arguments
	int first_arg = 1;
	while (argv[i])
	{
		if (!first_arg)
			printf(" ");
		else
			first_arg = 0;
		
		if (enable_escape)
		{
			char *expanded = expand_escape(argv[i]);
			if (expanded)
			{
				printf("%s", expanded);
				free(expanded);
			}
			else
				printf("%s", argv[i]);
		}
		else
			printf("%s", argv[i]);
		i++;
	}
	if (newline)
		printf("\n");
}

char *expand_escape(const char *str)
{
    // We'll allocate enough space for the worst case:
    char *result = malloc(ft_strlen(str) + 1);
    if (!result)
        return NULL;

    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] == '\\' && str[i + 1] != '\0')
        {
            i++;
            if (str[i] == 'n')
                result[j++] = '\n';
            else if (str[i] == 't')
                result[j++] = '\t';
            else
            {
                // Unknown escape => treat it literally
                result[j++] = '\\';
                result[j++] = str[i];
            }
        }
        else
        {
            result[j++] = str[i];
        }
        i++;
    }
    result[j] = '\0';
    return result;
}



