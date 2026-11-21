/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: 2026 Sven Eckelmann <sven@narfation.org>
 */

/*
 * fft_eval_sdl is written against the SDL3 API. This header provides the
 * small set of helpers which hide the differences to SDL2 when the SDL3
 * development files were not available during the build.
 */

#ifndef _FFT_EVAL_SDL_COMPAT_H
#define _FFT_EVAL_SDL_COMPAT_H

#include <stdbool.h>

#ifdef CONFIG_SDL3

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

static inline bool compat_sdl_init(void)
{
	return SDL_Init(SDL_INIT_VIDEO);
}

static inline SDL_Window *compat_create_window(const char *title, int w, int h)
{
	return SDL_CreateWindow(title, w, h, SDL_WINDOW_RESIZABLE);
}

static inline SDL_Renderer *compat_create_renderer(SDL_Window *window)
{
	return SDL_CreateRenderer(window, NULL);
}

static inline bool compat_ttf_init(void)
{
	return TTF_Init();
}

static inline SDL_Surface *compat_create_surface(int w, int h, int bpp,
						 Uint32 rmask, Uint32 gmask,
						 Uint32 bmask, Uint32 amask)
{
	SDL_PixelFormat format;

	format = SDL_GetPixelFormatForMasks(bpp, rmask, gmask, bmask, amask);
	if (format == SDL_PIXELFORMAT_UNKNOWN)
		return NULL;

	return SDL_CreateSurface(w, h, format);
}

static inline SDL_Surface *compat_render_text(TTF_Font *font, const char *text,
					      SDL_Color color)
{
	return TTF_RenderText_Solid(font, text, 0, color);
}

static inline void compat_render_texture(SDL_Renderer *renderer,
					 SDL_Texture *texture, int w, int h)
{
	SDL_FRect dest = {0, 0, w, h};

	SDL_RenderTexture(renderer, texture, NULL, &dest);
}

static inline SDL_Keycode compat_event_key(const SDL_Event *event)
{
	return event->key.key;
}

static inline bool compat_event_is_exposed(const SDL_Event *event)
{
	return event->type == SDL_EVENT_WINDOW_EXPOSED;
}

#else /* !CONFIG_SDL3 */

#include <SDL.h>
#include <SDL_ttf.h>

#define SDL_EVENT_FIRST		SDL_FIRSTEVENT
#define SDL_EVENT_QUIT		SDL_QUIT
#define SDL_EVENT_KEY_DOWN	SDL_KEYDOWN
#define SDL_EVENT_KEY_UP	SDL_KEYUP

#define SDLK_I			SDLK_i

#define SDL_DestroySurface	SDL_FreeSurface

static inline bool compat_sdl_init(void)
{
	return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0;
}

static inline SDL_Window *compat_create_window(const char *title, int w, int h)
{
	return SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, w, h,
				SDL_WINDOW_RESIZABLE);
}

static inline SDL_Renderer *compat_create_renderer(SDL_Window *window)
{
	return SDL_CreateRenderer(window, -1, 0);
}

static inline bool compat_ttf_init(void)
{
	return TTF_Init() == 0;
}

static inline SDL_Surface *compat_create_surface(int w, int h, int bpp,
						 Uint32 rmask, Uint32 gmask,
						 Uint32 bmask, Uint32 amask)
{
	return SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, bpp, rmask, gmask,
				    bmask, amask);
}

static inline SDL_Surface *compat_render_text(TTF_Font *font, const char *text,
					      SDL_Color color)
{
	return TTF_RenderText_Solid(font, text, color);
}

static inline void compat_render_texture(SDL_Renderer *renderer,
					 SDL_Texture *texture, int w, int h)
{
	SDL_Rect dest = {0, 0, w, h};

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

static inline SDL_Keycode compat_event_key(const SDL_Event *event)
{
	return event->key.keysym.sym;
}

static inline bool compat_event_is_exposed(const SDL_Event *event)
{
	return event->type == SDL_WINDOWEVENT &&
	       event->window.event == SDL_WINDOWEVENT_EXPOSED;
}

#endif /* CONFIG_SDL3 */

#endif /* _FFT_EVAL_SDL_COMPAT_H */
