#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "vibrant_logs.h"
#include "ez_strings.h"

EzString ez_string_create(const char *literal)
{
	EzString s = {0};

	if(!literal)
	{
		s.data = NULL;
		vl_log(VL_ERROR, "Cannot create EzString from null string literal.");
		return s;
	}

	size_t len = strlen(literal);
	s.capacity = len + 1;
	s.length = len;

	s.data = malloc(s.capacity);
	if(!s.data)
	{
		vl_log(VL_ERROR, "Failed to allocate memory for EzString.");
		return s;
	}

	memcpy(s.data, literal, s.capacity);

	return s;
}
EzString ez_string_malloc(size_t length)
{
	EzString s = {0};

	if(length == SIZE_MAX)
	{
		s.data = NULL;
		vl_log(VL_ERROR, "Failed to malloc for EzString, the given length is equal to SIZE_MAX.");
		return s;
	}

	s.capacity = length + 1;
	s.length = length;

	s.data = malloc(s.capacity);

	if(!s.data)
		vl_log(VL_ERROR, "Failed to allocate memory for EzString.");

	return s;
}
void ez_string_destroy(EzString *str)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot destroy nulll EzString.");
		return;
	}

	// free string memory if not null
	if(str -> data)
		free(str -> data);

	// reset members
	str -> data = NULL;
	str -> length = 0;
	str -> capacity = 0;
}

int ez_string_append_c(EzString *str, char c)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot append character to null/invalid EzString.");
		return 0;
	}

	// simply insert 'c' at end of 'str'
	ez_string_insert_c(str, c, str -> length);

	return 1;
}
int ez_string_append_s(EzString *str, const char *s)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot append string to null/invalid EzString.");
		return 0;
	}
	if(!s)
	{
		vl_log(VL_ERROR, "Cannot append null string to an EzString.");
		return 0;
	}

	// obtain length of string literal given
	size_t s_len = strlen(s);

	// should the EzString expand?
	if(str -> length + s_len + 1 >= str -> capacity)
	{
		size_t new_cap = str -> capacity ? str -> capacity : 1;

		// grow 'new_cap' until input string fits into the EzString
		while(str -> length + s_len + 1 > new_cap)
			new_cap *= 2;

		// new memory/data:
		char *new_data = realloc(str -> data, new_cap);
		if(!new_data)
		{
			vl_log(VL_ERROR, "Failed to realloc memory for the EzString.");
			return 0;
		}

		str -> data = new_data;
		str -> capacity = new_cap;
	}

	// append string
	memcpy(str -> data + str -> length, s, s_len);

	// add to length of EzString
	str -> length += s_len;

	// ensure EzString ends with null terminator character
	str -> data[str -> length] = '\0';

	return 1;
}

int ez_string_insert_c(EzString *str, char c, size_t index)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot insert character into null/invalid EzString.");
		return 0;
	}
	if(index > str -> length)
	{
		vl_log(VL_ERROR, "Invalid index when inserting character into EzString.");
		return 0;
	}

	// ensure there is enough capacity
	if(str -> length + 1 >= str -> capacity)
	{
		size_t new_cap = str -> capacity ? str -> capacity * 2 : 2;
		char *new_data = realloc(str -> data, new_cap);

		if(!new_data)
		{
			vl_log(VL_ERROR, "Failed to realloc memory for the EzString.");
			return 0;
		}

		str -> data = new_data;
		str -> capacity = new_cap;
	}

	// shift everything else to the right
	memmove(&str -> data[index + 1], &str -> data[index], str -> length - index + 1);

	// set the specific character at the index given
	str -> data[index] = c;

	// because a char was inserted, add to length
	str -> length++;

	return 1;
}
int ez_string_insert_s(EzString *str, const char *s, size_t index)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot insert string into null/invalid EzString.");
		return 0;
	}
	if(!s)
	{
		vl_log(VL_ERROR, "Cannot insert null string into an EzString.");
		return 0;
	}
	if(index > str -> length)
	{
		vl_log(VL_ERROR, "Invalid index when inserting string into EzString.");
		return 0;
	}

	// find length of input string literal
	size_t s_len = strlen(s);
	if(s_len == 0)
	{
		vl_log(VL_WARNING, "Inserting a string of 0 length results in the same EzString.");
		return 0;
	}

	// ensure there is enough capacity
	if(str -> length + s_len >= str -> capacity)
	{
		size_t new_cap = str -> capacity ? str -> capacity : 1;
		// grow until input string fits
		while(str -> length + s_len >= new_cap)
			new_cap *= 2;

		char *new_data = realloc(str -> data, new_cap);
		if(!new_data)
		{
			vl_log(VL_ERROR, "Failed to realloc memory for the EzString.");
			return 0;
		}

		str -> data = new_data;
		str -> capacity = new_cap;
	}

	// shift everything to right
	memmove(&str -> data[index + s_len], &str -> data[index], str -> length - index + 1);

	// copy new string into position
	memcpy(&str -> data[index], s, s_len);

	str -> length += s_len;

	return 1;
}

int ez_string_pop_back(EzString *str)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot remove last character from null/invalid EzString.");
		return 0;
	}
	if(str -> length == 0)
	{
		vl_log(VL_WARNING, "EzString is empty, cannot pop back.");
		return 0;
	}

	// remove 1 from length
	str -> length--;

	/* set the char in the array at the new length index to null terminator

	   this really means there are now two null terminators in the string since
	   the actual capacity of the string was unchanged. visually, the string
	   becomes shorter though.
	*/
	str -> data[str -> length] = '\0';

	return 1;
}
int ez_string_remove_c(EzString *str, char c)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot remove character from null/invalid EzString.");
		return 0;
	}
	if(str -> length == 0)
	{
		vl_log(VL_WARNING, "Cannot remove character from the given EzString because its length is 0.");
		return 0;
	}

	// find position of first occurence of 'c' in 'str'
	size_t pos = str -> length;
	for(size_t i = 0; i < str -> length; ++i)
	{
		if(str -> data[i] == c)
		{
			pos = i;
			break;
		}
	}

	// if char not found, do nothing
	if(pos == str -> length)
	{
		vl_log(VL_WARNING, "The char '%c' was not found in the EzString: '%s'", c, str -> data);
		return 0;
	}

	// shift all remaining characters to the left
	for(size_t i = pos; i < str -> length - 1; ++i)
		str -> data[i] = str -> data[i + 1];

	str -> length--;
	str -> data[str -> length] = '\0';

	return 1;
}
int ez_string_remove_all_c(EzString *str, char c)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot remove characters from null/invalid EzString.");
		return 0;
	}
	if(str -> length == 0)
	{
		vl_log(VL_WARNING, "Cannot remove characters from an empty EzString.");
		return 0;
	}

	size_t j = 0;
	// iterate over entire string
	for(size_t i = 0; i < str -> length; ++i)
	{
		// if the current character is not equal to the target character, keep it
		if(str -> data[i] != c)
			// overwrite char at j, then increment j
			str -> data[j++] = str -> data[i];
	}

	/* was 'c' found at all?

	   if c was not found, j should be equal to the string's current length.
	   this is because in the for loop above, we overwrite each non-matching char
	   at index j. that means if the character was not found, j reached
	   the end of the string.
	*/
	if(j == str -> length)
	{
		vl_log(VL_WARNING, "The char '%c' was not found in the EzString: '%s'", c, str -> data);
		return 0;
	}

	// the string's new length is equal to 'j'
	str -> length = j;
	str -> data[j] = '\0';

	return 1;
}
int ez_string_remove_s(EzString *str, const char *s)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot remove string from null/invalid EzString.");
		return 0;
	}
	if(!s)
	{
		vl_log(VL_ERROR, "Cannot remove null string from an EzString.");
		return 0;
	}

	// ensure input string is valid
	size_t s_len = strlen(s);
	if(s_len == 0 || str -> length < s_len)
	{
		vl_log(VL_ERROR, "Unable to remove the given string from the EzString due to an invalid length.");
		return 0;
	}

	// compare strings:
	for(size_t i = 0; i <= str -> length - s_len; ++i)
	{
		if(strncmp(&str -> data[i], s, s_len) == 0)
		{
			memmove(&str -> data[i], &str -> data[i + s_len], str -> length - (i + s_len) + 1);
			str -> length -= s_len;
			return 1;
		}
	}

	vl_log(VL_WARNING, "The string '%s' was not found in the EzString: '%s'", s, str -> data);
	return 0;
}
int ez_string_remove_at(EzString *str, size_t index)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot remove characters from null/invalid EzString.");
		return 0;
	}
	if(index > str -> length)
	{
		vl_log(VL_ERROR, "Invalid index when removing a character from an EzString.");
		return 0;
	}

	// remove character from string
	size_t j = 0;
	for(size_t i = 0; i < str -> length; ++i)
	{
		// if the index given and i do not match, keep that character
		if(index != i)
			// overwrite char at j with the char at i, then increment j
			str -> data[j++] = str -> data[i];
	}

	str -> length--;
	str -> data[str -> length] = '\0';

	return 1;
}

int ez_string_find_c(EzString *str, char c)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot find character in a null/invalid EzString.");
		return -1;
	}

	// find first occurence of 'c' in 'str'
	for(int i = 0; str -> data[i] != '\0'; ++i)
	{
		if(str -> data[i] == c)
			return i;
	}

	vl_log(VL_WARNING, "The char '%c' was not found in the EzString: '%s'", c, str -> data);
	return -1;
}
int ez_string_find_s(EzString *str, const char *s)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot find string in a null/invalid EzString.");
		return -1;
	}
	if(!s)
	{
		vl_log(VL_ERROR, "Cannot find null string in an EzString.");
		return -1;
	}

	// ensure input string is valid
	size_t s_len = strlen(s);
	if(s_len == 0 || str -> length < s_len)
	{
		vl_log(VL_ERROR, "Unable to remove the given string from the EzString due to an invalid length.");
		return -1;
	}

	// compare strings
	for(int i = 0; str -> data[i] != '\0'; ++i)
	{
		// if string found, return i
		if(strncmp(&str -> data[i], s, s_len) == 0)
			return i;
	}

	vl_log(VL_WARNING, "The string '%s' was not found in the EzString: '%s'", s, str -> data);
	return -1;
}
size_t ez_string_count_c(EzString *str, char c)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot count characters in null/invalid EzString.");
		return 0;
	}

	size_t count = 0;

	for(size_t i = 0; i < str -> length; ++i)
	{
		if(str -> data[i] == c)
			count++;
	}

	return count;
}
size_t ez_string_count_s(EzString *str, const char *s)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot count strings in null/invalid EzString.");
		return 0;
	}
	if(!s)
	{
		vl_log(VL_ERROR, "Cannot search for null string in an EzString.");
		return 0;
	}

	// find len of 's'
	size_t s_len = strlen(s);

	size_t count = 0;

	for(size_t i = 0; i < str -> length; ++i)
	{
		// compare strings
		if(strncmp(&str -> data[i], s, s_len) == 0)
			count++;
	}

	return count;
}

int ez_string_replace_c(EzString *str, char target, char new)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot replace characters in null/invalid EzString.");
		return 0;
	}
	if(str -> length == 0)
	{
		vl_log(VL_WARNING, "The given EzString is empty, cannot replace characters.");
		return 0;
	}

	// find 'target' in 'str' and replace with 'new'
	size_t pos = str -> length;
	for(size_t i = 0; i < str -> length; ++i)
	{
		if(str -> data[i] == target)
		{
			pos = i;
			break;
		}
	}

	// if char not found, do nothing
	if(pos == str -> length)
	{
		vl_log(VL_WARNING, "The char '%c' was not found in the EzString: '%s'", target, str -> data);
		return 0;
	}

	// replace char
	str -> data[pos] = new;

	return 1;
}
int ez_string_replace_all_c(EzString *str, char target, char new)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot replace characters in null/invalid EzString.");
		return 0;
	}
	if(str -> length == 0)
	{
		vl_log(VL_WARNING, "The given EzString is empty, cannot replace characters.");
		return 0;
	}

	// iterate over entire string and change all matching characters
	bool target_found = false;
	for(size_t i = 0; i < str -> length; ++i)
	{
		if(str -> data[i] == target)
		{
			str -> data[i] = new;
			target_found = true;
		}
	}

	if(!target_found)
	{
		vl_log(VL_WARNING, "The char '%c' was not found in the EzString: '%s'", target, str -> data);
		return 0;
	}

	return 1;
}
int ez_string_replace_s(EzString *str, const char *target, const char *new)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot replace characters in null/invalid EzString.");
		return 0;
	}
	if(!target)
	{
		vl_log(VL_ERROR, "Target string is invalid.");
		return 0;
	}
	if(!new)
	{
		vl_log(VL_ERROR, "New string is invalid.");
		return 0;
	}

	// find 'target' in 'str'
	size_t target_len = strlen(target);
	size_t target_pos = str -> length;

	// ensure lengths are valid
	if(target_len > str -> length)
	{
		vl_log(VL_ERROR, "Target length exceeds EzString length.");
		return 0;
	}

	for(size_t i = 0; i <= str -> length - target_len; ++i)
	{
		if(strncmp(&str -> data[i], target, target_len) == 0)
		{
			target_pos = i;
			break;
		}
	}

	// was target found?
	if(target_pos == str -> length)
	{
		vl_log(VL_WARNING, "The string '%s' was not found in the EzString: '%s'", target, str -> data);
		return 0;
	}

	// replace 'target' with 'new'
	size_t new_len = strlen(new);

	// shift remainder of string
	memmove(&str -> data[target_pos + new_len], &str -> data[target_pos + target_len], str -> length - (target_pos + target_len) + 1);

	// copy replacement string
	memcpy(&str -> data[target_pos], new, new_len);

	// update string length
	str -> length = str -> length - target_len + new_len;

	return 1;
}

int ez_string_trim(EzString *str)
{
	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot trim null/invalid EzString.");
		return 0;
	}
	if(str -> length == 0)
	{
		vl_log(VL_WARNING, "Cannot trim EzString with length of 0.");
		return 0;
	}

	char *original = str -> data;

	// left trim first:
	char *start = str -> data;
	while(*start && isspace((unsigned char) *start))
		start++;

	// keep track of whether or not trimming succeeded
	int trimmed = (start != original);

	// if left trim succeeded but end of string was reached, then the string was all whitespace
	if(*start == '\0')
	{
		if(original[0] != '\0')
		{
			original[0] = '\0';
			str -> length = 0;
			return 1;
		}

		return 0;
	}

	// right trim:
	char *end = start + strlen(start) - 1;
	while(end > start && isspace((unsigned char) *end))
		end--;

	if(end[1] != '\0')
		trimmed = 1;

	// end points to last char in new string, so the next char ([1]) should be the null terminator
	end[1] = '\0';

	// shift trimmed string to beginning of memory block
	if(start != original)
		memmove(str -> data, start, strlen(start) + 1);

	// reset string length
	str -> length = strlen(str -> data);

	return trimmed;
}

EzStrings ez_string_split(EzString *str, char delimiter)
{
	EzStrings zero_init = {0};
	EzStrings strings = {0};

	if(!str || !str -> data)
	{
		vl_log(VL_ERROR, "Cannot split null/invalid EzString.");
		return zero_init;
	}

	// find count of 'delimiter' in 'str'
	size_t delim_count = 0;
	for(size_t i = 0; i < str -> length; ++i)
	{
		if(str -> data[i] == delimiter)
			delim_count++;
	}

	// if delim_count was 0, then no strings can be split
	if(delim_count == 0)
		return zero_init;

	// the number of strings split is always the number of delimiters found + 1
	size_t num_strings = delim_count + 1;
	strings.count = num_strings;

	// allocate memory to fit the num strings
	EzString *split_strings = malloc(sizeof(EzString) * num_strings);
	if(!split_strings)
	{
		vl_log(VL_ERROR, "Failed to allocate memory for EzString array.");
		return zero_init;
	}

	// append each split string to array

	size_t start = 0;
	size_t split_index = 0;

	for(size_t i = 0; i <= str -> length; ++i)
	{
		if(i == str -> length || str -> data[i] == delimiter)
		{
			size_t part_len = i - start;

			// allocate for substring
			split_strings[split_index].data = malloc(part_len + 1);

			if(!split_strings[split_index].data)
			{
				vl_log(VL_ERROR, "Failed to allocate memory for EzString.");

				// clean up allocated strings
				for(size_t j = 0; j < split_index; ++j)
					free(split_strings[j].data);

				free(split_strings);
				return zero_init;
			}

			// copy substring
			memcpy(split_strings[split_index].data, &str -> data[start], part_len);

			// null terminate
			split_strings[split_index].data[part_len] = '\0';

			// set length and cap of split string
			split_strings[split_index].length = part_len;
			split_strings[split_index].capacity = part_len + 1;

			// move to next split string index
			split_index++;

			// next substring starts after delimiter
			start = i + 1;
		}
	}

	strings.data = split_strings;

	return strings;
}
void ez_string_destroy_split(EzStrings *strings)
{
	if(!strings)
	{
		vl_log(VL_ERROR, "Cannot destroy null split string array.");
		return;
	}

	for(size_t i = 0; i < strings -> count; ++i)
		ez_string_destroy(&strings -> data[i]);

	free(strings -> data);
}

void ez_string_print(EzString *str)
{
	if(!str || !str -> data)
		vl_log(VL_DEBUG, "EzString: NULL");
	else
		vl_log(VL_DEBUG, "EzString: '%s'", str -> data);
}
void ez_string_print_split(EzStrings *strings)
{
	if(!strings)
		vl_log(VL_ERROR, "EzString Split Array: NULL");
	else
		for(size_t i = 0; i < strings -> count; ++i)
			vl_log(VL_DEBUG, "EzString Split Array[%zu]: '%s'", i, strings -> data[i].data);
}
