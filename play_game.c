/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wportilh <wportilh@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 16:03:00 by acosta-a          #+#    #+#             */
/*   Updated: 2023/01/04 00:55:00 by wportilh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

# define WIDTH 320
# define HEIGHT 200

/*Adiciona pixel por pixel nas coordenadas que damos*/
static void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->wdt + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

/*Adiciona um fundo à tela (preenche a metade da tela)*/
static void	fill_background(t_game *game)
{
	int	x;
	int	y;

	y = -1;
	while (++y < (HEIGHT / 2))
	{
		x = -1;
		while (++x < WIDTH)
			my_mlx_pixel_put(&game->img, x, y, 0XBEBEFF);
	}
	--y;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
			my_mlx_pixel_put(&game->img, x, y, 0X696969);
	}
}

/* Inicializa as configurações do player
*/
static void	init_player(t_game *game)
{
	game->player.pos[0] = 5.5; // posição x do player no mapa;
	game->player.pos[1] = 5.5; // posição y do player no mapa;
	game->player.abs_pos[0] = abs(game->player.pos[0]); // posição x absoluta do player no mapa;
	game->player.abs_pos[1] = abs(game->player.pos[1]); // posição y absoluta do player no mapa;
	game->player.dir[0] = 0; // posição x do vetor dir no mapa (não muda);
	game->player.dir[1] = -1; // posição y do vetor dir no mapa (não muda);
	game->player.plane[0] = 0.66; // posição x do vetor plane no mapa (não muda);
	game->player.plane[1] = 0; // posição y do vetor plane no mapa (não muda);
}

/*	Cálculo dos raios.
	Quando calculo camera_pixel, não aloco o y com malloc, porque o valor dele não muda.
	game->player.plane[1] é o y, que sempre é 0. (se precisar no futuro, talvez na rotação, aloco ele também).
	Ou seja, o que interessa é calcular x, que varia.
	
	A mesma coisa se aplica ao ray_dir, mas acho que na rotação vou precisar (ou não kkk) alocar ele.
*/
static void	calc_ray_dir(t_game *game)
{
	float	i;
	float	vetor_size;
	float	camera_pixel_x;
	float	camera_pixel_y;

	game->ray.ray_dir_x = malloc((WIDTH + 1) * sizeof(float));
	if (!game->ray.ray_dir_x)
		exit (EXIT_FAILURE);
	i = -1;
	while (++i <= WIDTH)
	{
		vetor_size = 2 * (i/WIDTH) - 1; // Calcula o tamanho do vetor dos raios em relação à player.plane
		camera_pixel_x = (vetor_size * game->player.plane[0]); // (multiplicação de vetores) Calcula a posição dos vetores x dos raios
		game->ray.ray_dir_x[(int)i] = camera_pixel_x + game->player.dir[0]; // (soma de vetores) Calcula a direção x dos raios
	}
	camera_pixel_y = vetor_size * game->player.plane[1]; // (multiplicação de vetores) Calcula a posição dos vetores y dos raios
	game->ray.ray_dir_y = camera_pixel_y + game->player.dir[1]; // (soma de vetores) Calcula a direção y dos raios.
}

/*	Cálculo dos deltas, que são as distâncias x e y usadas no algoritmo DDA para detectar os muros.
	No caso do x, obteremos a distância para x chegar a alguma linha vertical.
	No caso do y, obteremos a distância para y chegar a alguma linha horizontal.
*/
void	calc_delta_dist_x_and_y(t_game *game)
{
	int		i;
	float	ray_dir_hypotenuse;

	game->delta.delta_dist_x = malloc((WIDTH + 1) * sizeof(float));
	if (!game->delta.delta_dist_x)
		exit (EXIT_FAILURE);
	game->delta.delta_dist_y = malloc((WIDTH + 1) * sizeof(float));
	if (!game->delta.delta_dist_y)
		exit (EXIT_FAILURE);
	i = -1;
	while (++i < WIDTH)
	{
		ray_dir_hypotenuse = sqrt(pow(game->ray.ray_dir_x[i], 2) + pow(game->ray.ray_dir_y, 2)); // Magnitude do ray dir
		game->delta.delta_dist_x[i] = ray_dir_hypotenuse / game->ray.ray_dir_x[i]; // magnitude do ray_dir dividida por ray dir_x (semelhança de triângulos)
		game->delta.delta_dist_y[i] = ray_dir_hypotenuse / game->ray.ray_dir_y; // magnitude do ray_dir dividida por ray dir_y (semelhança de triângulos)
		//printf("   i: %d\n", i);
		//printf("hypo: %f\n", ray_dir_hypotenuse);
		//printf("rayx: %f\n", game->ray.ray_dir_x[i]);
		//printf("rayy: %f\n", game->ray.ray_dir_y);
		//printf("   x: %f\n", game->delta.delta_dist_x[i]); // prints para testes
		//printf("   y: %f\n", game->delta.delta_dist_y[i]);
	}
}

void	calc_dist_to_side_x_and_y(t_game *game)
{
	int	i;

	game->dist.dist_to_side_x = malloc((WIDTH + 1) * sizeof(float));
	if (!game->dist.dist_to_side_x)
		exit (EXIT_FAILURE);
	game->dist.dist_to_side_y = malloc((WIDTH + 1) * sizeof(float));
	if (!game->dist.dist_to_side_y)
		exit (EXIT_FAILURE);
	i = -1;
	while (++i < WIDTH)
	{
		if (game->ray.ray_dir_x <= 0)
			game->dist.dist_to_side_x[i] = (game->player.pos[0] \
			- game->player.abs_pos[0]) * game->delta.delta_dist_x[i];
		else
			game->dist.dist_to_side_x[i] = (game->player.abs_pos[0] + 1 \
			- game->player.pos[0]) * game->delta.delta_dist_x[i];
		if (game->ray.ray_dir_y <= 0)
			game->dist.dist_to_side_y[i] = (game->player.pos[1] \
			- game->player.abs_pos[1]) * game->delta.delta_dist_y[i];
		else
			game->dist.dist_to_side_y[i] = (game->player.abs_pos[1] + 1 \
			- game->player.pos[1]) * game->delta.delta_dist_y[i];
		printf("result: %f\n", game->dist.dist_to_side_x[i]);
		printf("result: %f\n", game->dist.dist_to_side_y[i]);
	}
}

void	play_game(t_game *game)
{
	game->mlx = mlx_init();
	game->window = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3d");
	game->img.img_ptr = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->img.addr = mlx_get_data_addr(game->img.img_ptr, &game->img.bpp, &game->img.wdt, &game->img.endian);
	fill_background(game);
	init_player(game);
	calc_ray_dir(game);
	calc_delta_dist_x_and_y(game);
	calc_dist_to_side_x_and_y(game);
	mlx_put_image_to_window(game->mlx, game->window, game->img.img_ptr, 0, 0);
	mlx_loop(game->mlx);
}
