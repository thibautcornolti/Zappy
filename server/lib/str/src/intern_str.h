/*
** EPITECH PROJECT, 2018
** irc
** File description:
** intern_str
*/

#pragma once

#include <stdbool.h>

bool lstr_intern_match(const char *s1, const char *s2, int idx1, int idx2);
bool lstr_intern_match_helper(
	const char *s1, const char *s2, int idx1, int idx2);
