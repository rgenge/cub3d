/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 16:28:40 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/23 22:23:23 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

//função pra printar a saida e economizar linhas
void	print_exit(char *input)
{
	printf("%s", input);
	exit (1);
}

void	clean_exit(t_game *game)
{
	destroy_pointers_char(game->map);
	free(game->no_wall_path);
	free(game->so_wall_path);
	free(game->we_wall_path);
	free(game->ea_wall_path);
	free(game->c_inputs);
	free(game->f_inputs);
	exit (1);
}

//função pra dar free em vetores
void	destroy_pointers_char(char **p)
{
	int	i;

	i = 0;
	while (p != NULL && p[i] != NULL)
		free(p[i++]);
	free(p);
}
//função utilizada pra printar o vetor apenas pra checar se funciona
void	print_vector(char **p)
{
	int	i;

	i = 0;
	while (p != NULL && p[i] != NULL)
		printf("%s\n", p[i++]);
}
