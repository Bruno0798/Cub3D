/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-souz <bde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:42:38 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/09/10 15:55:17 by bde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/cub3d.h"
#include <assert.h>

static void ft_extend_map(t_game *game)
{
	int i = 0;
	char **new_map = malloc(sizeof(char *) * (game->map.height + 2));
	if (new_map == NULL)
		return ; //TODO change this function to deal with leaks and print error
	while (game->map.map[i] != NULL)
	{
		new_map[i] = ft_strdup(game->map.map[i]);
		i++;
	}
	new_map[i] = NULL;
	i = 0;
	while(game->map.map[i])
	{
		free(game->map.map[i]);
		i++;
	}
	free(game->map.map);
	game->map.map = new_map;
}

static bool ft_check_empty_line(const char *line, int option)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (option == 1)
		{
			if (line[i] != ' ' && line[i] != '\n')
				return false;
		}
		else
		{
			if (line[i] != '\n')
				return false;
		}
		i++;
	}
	return true;
}

static void ft_get_map(t_game *game)
{
	game->map.map = malloc(sizeof (char *) * 2);
	if (game->map.map == NULL)
		return ; // TODO CHANGE TO FUNCTION TO PRINT AND EXIT
	while(game->map.line != NULL)
	{
		if(!ft_check_empty_line(game->map.line, 2))
		{
			game->map.map[game->map.height] = ft_strdup(game->map.line);
			game->map.height++;
			game->map.map[game->map.height] =  NULL;
			ft_extend_map(game);
		}
		free(game->map.line);
		game->map.line = get_next_line(game->fd_file);
	}
}

static bool ft_all_textures_set(t_game *game)
{
	if (game->map.NORTH_PATH != NULL && game->map.SOUTH_PATH != NULL
		&& game->map.WEST_PATH != NULL && game->map.EAST_PATH != NULL
		&& game->map.CEILING_PATH != NULL && game->map.FLOOR_PATH != NULL)
	{
		return true;
	}
	return false;
}

static char	*remove_all_spaces(char *str)
{
	int i;
	int j;
	char *result;

	i = 0;
	j = 0;
	result = (char *) malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i] != ' ' && !ft_isalpha(str[i]))
		i++;
	while (ft_isalpha(str[i]))
		i++;
	while (str[i]) {
		if (str[i] != ' ')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	str = ft_strdup(result);
	free(result);
	return (str);
}

static int ft_set_texture(t_game *game, char *line)
{
	size_t len;

	len = ft_strlen(line);

	if(ft_strnstr(line, NORTH, len))
		game->map.NORTH_PATH = remove_all_spaces(line);
	else if(ft_strnstr(line, SOUTH, len))
		game->map.SOUTH_PATH = remove_all_spaces(line);
	else if(ft_strnstr(line, EAST, len))
		game->map.EAST_PATH = remove_all_spaces(line);
	else if(ft_strnstr(line, WEST, len))
		game->map.WEST_PATH = remove_all_spaces(line);
	else if(ft_strnstr(line, CEILING, len))
		game->map.CEILING_PATH = remove_all_spaces(line);
	else if(ft_strnstr(line, FLOOR, len))
		game->map.FLOOR_PATH = remove_all_spaces(line);
	else
		return 1;
	return 0;
}

static bool ft_check_duplicates(t_game *game, char *line)
{
	size_t len;

	len = ft_strlen(line);

	if(ft_strnstr(line, NORTH, len) && game->map.NORTH_PATH != NULL)
		return true;
	else if(ft_strnstr(line, SOUTH, len)&& game->map.SOUTH_PATH != NULL)
		return true;
	else if(ft_strnstr(line, EAST, len)&& game->map.EAST_PATH != NULL)
		return true;
	else if(ft_strnstr(line, WEST, len)&& game->map.WEST_PATH != NULL)
		return true;
	else if(ft_strnstr(line, CEILING, len)&& game->map.CEILING_PATH != NULL)
		return true;
	else if(ft_strnstr(line, FLOOR, len)&& game->map.FLOOR_PATH != NULL)
		return true;
	else
		return false;
}

static void ft_get_textures(t_game *game)
{

	game->map.line = get_next_line(game->fd_file);
	while (game->map.line != NULL && !ft_all_textures_set(game))
	{
		if (!ft_check_empty_line(game->map.line, 1))
		{
		    if(ft_check_duplicates(game, game->map.line))
			{
				printf("Error: duplicate textures");
				return ;
			}
			ft_set_texture(game, game->map.line);
		}
		free(game->map.line);
		game->map.line = get_next_line(game->fd_file);
	}
	// printf("%s", game->map.NORTH_PATH);
	// printf("%s", game->map.SOUTH_PATH);
	// printf("%s", game->map.EAST_PATH);
	// printf("%s", game->map.WEST_PATH);
	// printf("%s", game->map.CEILING_PATH);
	// printf("%s", game->map.FLOOR_PATH);
	while(game->map.line != NULL && ft_check_empty_line(game->map.line, 1))
	{
		free(game->map.line);
		game->map.line = get_next_line(game->fd_file);
	}
}

t_game *ft_init_struct(t_game *game, char *file)
{
	// game->mlx_ptr = NULL;
	// game->win_ptr = NULL;
	game->fd_file = open(file, O_RDONLY);
	game->map.NORTH_PATH = NULL;
	game->map.SOUTH_PATH = NULL;
	game->map.WEST_PATH = NULL;
	game->map.EAST_PATH = NULL;
	game->map.CEILING_PATH = NULL;
	game->map.FLOOR_PATH = NULL;
	game->map.height = 0;
	game->map.map = NULL;
	ft_get_textures(game);
	ft_get_map(game);
	return game;
}