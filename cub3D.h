/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wportilh <wportilh@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:41:53 by acosta-a          #+#    #+#             */
/*   Updated: 2023/01/04 21:15:33 by wportilh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <math.h>
# include "mlx.h"
# include "libft/libft.h"
# include "minilibx-linux/mlx.h"

# define TRUE 1 // Vou precisar em algumas funções
# define FALSE 0 // Essa também kkk

typedef struct s_dda
{
	int		*step_x;
	int		*step_y;
	int		*hit_side;
	float	*wall_map_pos_x;
	float	*wall_map_pos_y;
	float	*dda_line_size_x;
	float	*dda_line_size_y;
	float	*perpendicular_ray;
}					t_dda;

typedef struct s_dist
{
	float	*dist_to_side_x;
	float	*dist_to_side_y;
}					t_dist;

typedef struct s_delta
{
	float	*delta_dist_x;
	float	*delta_dist_y;
}					t_delta;

typedef struct s_ray
{
	float	*ray_dir_x;
	float	ray_dir_y;
}					t_ray;

typedef struct s_player
{
	int		dir[2];
	float	pos[2];
	float	plane[2];
}					t_player;	

typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		*data;
	int		wdt;
	int		hgt;
	int		size_l;
	int		bpp;
	int		endian;
}	t_img;

typedef struct s_textu
{
	t_img			img;
}					t_textu;

typedef struct s_rgb
{
	int				r;
	int				g;
	int				b;
}					t_rgb;

typedef struct s_game
{
	int				settings_count;
	void			*mlx;
	void			*window;
	char			*no_wall_path;
	char			*so_wall_path;
	char			*we_wall_path;
	char			*ea_wall_path;
	char			*f_inputs;
	char			*c_inputs;
	t_rgb			c;
	t_rgb			f;
	t_textu			textu[4];
	char			**map;
	t_img			img;
	int				i;
	t_player		player;
	t_ray			ray;
	t_delta			delta;
	t_dist			dist;
	t_dda			dda;
}	t_game;

/* cub3D.c*/

/* exit_utils.c*/
void	print_exit(char *input);
void	destroy_pointers_char(char **p);
void	clean_exit(t_game *game);
void	print_vector(char **p);

/* parse_settings.c*/
void	parse_settings(t_game *game, char **map);

/* parse_utils.c*/
char	*remove_space(char *str);

/* parse_utils.c*/
int		map_error(char *message, t_game *game);

/* parse_map.c*/
void	parse_map(t_game *game);

/* parse_map_2.c*/
void	check_walls_1(t_game *game);

/* parse_map_2.c*/
void	check_walls_2(t_game *game);

/* parse_map_2.c*/
void	check_corners(t_game *game);

/* play_game.c*/
void	play_game(t_game *game);;

#endif
