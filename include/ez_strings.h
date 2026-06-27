#pragma once

#ifdef EZ_STRINGS_EXPORTS
	#define EZ_STRINGS_API __declspec(dllexport)
#elif defined(EZ_STRINGS_STATIC)
	#define EZ_STRINGS_API
#else
	#define EZ_STRINGS_API __declspec(dllimport)
#endif

/**
  Represents a string. It holds the basic information
  about a string, such as the array of characters, the
  length of the string, etc.
*/
typedef struct ez_string
{
	/**
	  The actual array of characters in the string.
	*/
	char *data;
	/**
	  The length of the string.
	*/
	size_t length;
	/**
	  The max length of the string, or the string's
	  capacity.
	*/
	size_t capacity;
} ez_string;

/**
  Creates a new ez_string from a string literal.

  @note You must call ez_string_destroy(...) later.

  @note If the string literal given is NULL, then this
  function returns an ez_string with null/invalid data.
*/
EZ_STRINGS_API ez_string ez_string_create(const char *literal);
/**
  Creates a new ez_string that fits the length given.

  For example, if 5 is passed in here, the string returned will
  be able to hold 5 characters (excluding null terminator character).

  The length of the string is not final though; the string can still
  be edited.

  @note If the given length is equal to SIZE_MAX, then the function
  returns an ez_string with null/invalid data.

  @note You must call ez_string_destroy(...) later.
*/
EZ_STRINGS_API ez_string ez_string_malloc(size_t length);
/**
  Destroys an ez_string.

  If the given string is invalid or NULL, then nothing happens.
*/
EZ_STRINGS_API void ez_string_destroy(ez_string *str);

/**
  Appends a character to an ez_string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_append_c(ez_string *str, char c);
/**
  Appends a string to an ez_string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_append_s(ez_string *str, const char *s);

/**
  Inserts a character into an ez_string at a given index.

  @note The index can be equal to the length of the ez_string,
  and the character is simply appended to the string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_insert_c(ez_string *str, char c, size_t index);
/**
  Inserts a string into an ez_string at a given index.

  @note The index can be equal to the length of the ez_string,
  and the string literal given is simply appended to the string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_insert_s(ez_string *str, const char *s, size_t index);

/**
  Removes the last character from an ez_string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_pop_back(ez_string *str);
/**
  Removes the first occurence of a character in an ez_string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_remove_c(ez_string *str, char c);
/**
  Removes all occurences of a character in an ez_string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_remove_all_c(ez_string *str, char c);
/**
  Removes the first occurence of a string in an ez_string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_remove_s(ez_string *str, const char *s);
/**
  Removes the character at a given index in an ez_string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_remove_at(ez_string *str, size_t index);

/**
  Finds the index of the first occurence of a character in an ez_string.

  If the character is not found in the string or if the given
  ez_string is null/invalid, then -1 is returned.
*/
EZ_STRINGS_API int ez_string_find_c(ez_string *str, char c);
/**
  Finds the index of the first occurence of a string in an ez_string.

  If the string is not found in the ez_string or if any of the given
  data is null/invalid, then -1 is returned.
*/
EZ_STRINGS_API int ez_string_find_s(ez_string *str, const char *s);
/**
  Finds the number of times a character occurs in an ez_string.

  @return 0 if 'c' is not found in the string, or if the given
  ez_string is null/invalid in any way.
*/
EZ_STRINGS_API size_t ez_string_count_c(ez_string *str, char c);
/**
  Finds the number of times a string occurs in an ez_string.

  @return 0 if 's' is not found in the ez_string, or if the given
  ez_string is null/invalid in any way.
*/
EZ_STRINGS_API size_t ez_string_count_s(ez_string *str, const char *s);

/**
  Replaces the first occurence of a character in an ez_string with another character.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_replace_c(ez_string *str, char target, char new);
/**
  Replaces all occurences of a character in an ez_string with another character.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_replace_all_c(ez_string *str, char target, char new);
/**
  Replaces the first occurence of a string in an ez_string with another string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_replace_s(ez_string *str, const char *target, const char *new);

/**
  Trims an ez_string.

  Trimming an ez_string removes all leading and trailing whitespace
  from the string.

  @return 1 on success, 0 on failure.
*/
EZ_STRINGS_API int ez_string_trim(ez_string *str);

/**
  Represents an array of ez_strings.

  An array of ez_strings is typically used
  when splitting another ez_string.
*/
typedef struct ez_strings
{
	/**
	  The array of ez_strings.
	*/
	ez_string *data;
	/**
	  The number of strings in the array.
	*/
	size_t count;
} ez_strings;

/**
  Splits a string based on a delimiter.

  Splitting a string results in an array of strings.

  @important If the splitting fails, then NULL is returned instead.
  If a valid pointer is returned, it will need to be
  destroyed later with ez_string_destroy_split(...).
*/
EZ_STRINGS_API ez_strings ez_string_split(ez_string *str, char delimiter);
/**
  Destroys a split ez_strings array.

  @param arr The split array returned by ez_string_split(...).
  @param count The number of split elements. ez_string_split(...)
  will return this in the form of 'out_num_strings.'
*/
EZ_STRINGS_API void ez_string_destroy_split(ez_strings *strings);

/**
  Prints an ez_string, or "NULL" if it is null/invalid in any way.
*/
EZ_STRINGS_API void ez_string_print(ez_string *str);
/**
  Prints an ez_string split array.
*/
EZ_STRINGS_API void ez_string_print_split(ez_strings *strings);
