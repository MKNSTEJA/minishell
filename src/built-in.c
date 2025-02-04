#include "../include/minishell.h"


void print_exported_environ(char **envp)
{
    for (int i = 0; envp[i]; i++)
    {
        // We print it in "declare -x KEY=VALUE" style (bash-like).
        // Optional: parse out KEY and VALUE to insert quotes around VALUE.
        // For a simpler approach, just do:
        printf("declare -x %s\n", envp[i]);
    }
}

void set_env_variable(const char *key, const char *value, char ***envp)
{
	int i = 0;
    int   key_len = ft_strlen(key);

    // 1) Search existing environment for the key
    while ((*envp)[i])
    {
        if (ft_strncmp((*envp)[i], key, key_len) == 0 && (*envp)[i][key_len] == '=')
        {
            // 2) Found => replace old "KEY=old_value" with "KEY=new_value"
            free((*envp)[i]);
            (*envp)[i] = create_env_string(key, value);
            return;
        }
		i++;
    }
    // 3) Not found => add new environment variable
    add_env_variable(key, value, envp);
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
char *my_getenv(const char *name, char **env)
{
    int i = 0;
    int len = ft_strlen(name);
    while (env[i])
    {
        if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
            return env[i] + len + 1;
        i++;
    }
    return NULL;
}


void add_env_variable(const char *key, const char *value, char ***envp)
{

    // 1) Count how many entries in environ
    int i = 0;
    while ((*envp)[i])
        i++;

    // 2) Allocate new array (old_count + 2),
    //    because we need one extra for the new var and one for NULL terminator.
    char **new_env = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_env)
        return;

    // 3) Copy old environ pointers
    for (int j = 0; j < i; j++)
        new_env[j] = (*envp)[j];

    // 4) Create "KEY=VALUE" and append
    new_env[i] = create_env_string(key, value);
    new_env[i + 1] = NULL;

    // Optionally, free the old array if you're sure you allocated it
    // in your own code.  Depending on your project specs, you might do:
    // free(environ);
    // but in some setups, 'environ' might be statically allocated by the system.
    // In 42 projects, typically we've duplicated environ at startup,
    // so we can safely free it. Adjust to your code:
    free(*envp);

    // 6) Point `environ` to the new array
    *envp = new_env;
}


void    handle_export(char **argv, t_data *data)
{
	int i = 1;
    // If "export" has no arguments, just display the environment
    if (!argv[1])
    {
        print_exported_environ(data->env);
        return;
    }
	
    while (argv[i])
    {
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
            set_env_variable(key, value, &(data->env));

            free(key);
            free(value);
        }
        else
        {
            // No '=' => "export KEY"
            // By default, let's set KEY to empty if it doesn't exist
            // or do nothing if it already exists
            set_env_variable(argv[i], "", &(data->env));
        }
		i++;
    }
}

void handle_exit(char **argv)
{
    int status = 0;
	int count = 0;
	while (argv[count])
		count++;
	
    if (count > 1)
	{
		if (!is_numeric(argv[1]))
		{
			print_error_msg("exit", argv[1], "numeric argument required");
            exit(255);
		}
		else if (count > 2)
		{
			print_error_msg("exit", NULL, "too many arguments");
			return;
		}
		else
		{
			status = ft_atoi(argv[1]) % 256;
			if (status < 0)
				status += 256;
		}
	}
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

void handle_cd(char **argv, char ***envp)
{
    char *old_pwd = getcwd(NULL, 0);
    if (!old_pwd) 
	{
        perror("getcwd");
        return;
    }
    
    if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
    {
        char *home = my_getenv("HOME", *envp);
        if (home)
        {
            if (chdir(home) == 0)
            {
                char *cwd = getcwd(NULL, 0);
                if (cwd)
                {
                    set_env_variable("OLDPWD", old_pwd, envp);
                    set_env_variable("PWD", cwd, envp);
                    free(cwd);
                }
            }
            else
            {
                perror("cd");
            }
        }
        else
        {
            fprintf(stderr, "cd: HOME environment variable is not set\n");
        }
        free(old_pwd);
        return;
    }
    else if (argv[1][0] == '-' && argv[1][1] == '\0')
    {
        char *oldpwd_env = my_getenv("OLDPWD", *envp);
        if (!oldpwd_env)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            free(old_pwd);
            return;
        }
        if (chdir(oldpwd_env) == 0)
        {
            char *cwd = getcwd(NULL, 0);
            if (cwd)
            {
                set_env_variable("OLDPWD", old_pwd, envp);
                set_env_variable("PWD", cwd, envp);
                printf("%s\n", cwd);
                free(cwd);
            }
        }
        else
        {
            perror("cd");
        }
        free(old_pwd);
        return;
    }
    else
    {
        if (chdir(argv[1]) == 0)
        {
            char *cwd = getcwd(NULL, 0);
            if (cwd)
            {
                set_env_variable("OLDPWD", old_pwd, envp);
                set_env_variable("PWD", cwd, envp);
                free(cwd);
            }
        }
        else
        {
            fprintf(stderr, "cd: no such file or directory: %s\n", argv[1]);
        }
        free(old_pwd);
    }
}




void handle_unset(char **argv, char **envp)
{
	(void)argv;
	if (!argv[1])
	{
		const char *error_msg = "unset: Missing argument\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		return;
	}
	char **env = envp;
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

void handle_env(char **argv, t_data *data)
{
    char **env;

    if (argv && argv[1]) {
        print_error_msg("env", NULL, "too many arguments");
        return;
    }
    env = data->env;
    if (!env) {
        return;
    }
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
}

void handle_pwd(char **argv, char **envp)
{
	(void)argv;
	char *pwd = my_getenv("PWD", envp);
    if (pwd)
        printf("%s\n", pwd);
    else
	{
        const char *error_msg = "pwd: Unable to retrieve current directory\n";
    	write(STDERR_FILENO, error_msg, strlen(error_msg));
	}
}

int is_n_flag(char *arg)
{
	int i;

	i = 0;
	if (arg[i] != '-')
	{
		return (0);
	}
	i++;
	while (arg[i] == 'n')
	{
		i++;
	}
	return (arg[i] == '\0');
}
void handle_echo(char **argv)
{
	int i = 1; //start from 1 bec. 0 is the command itself
	int newline = 1; //default is to print newline

	// 1) parse flags
	while (argv[i] && is_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}

	int first_arg = 1;
	while (argv[i])
	{
		if (!first_arg)
			printf(" ");
		else
			first_arg = 0;

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



