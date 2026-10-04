/* Trim
Quick and dirty application to remove a prefix from multiple files' names.
Will print the potential changes and ask the user for confirmation before
any files are touched.
What happens if attempting to rename to a file that already exists should
depend on the standard library implementation, but in practice it would
probably just crash the programme.

-d to perform dry run.

Non-recursive.

Author: lfknudsen
*/

#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

string trim_whitespace_left(string input) {
	return input.erase(0, input.find_first_not_of(' '));
}

string trim_whitespace_right(string input) {
	return input.substr(0, input.find_last_not_of(' ') + 1);
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		cerr << "Please provide a prefix to replace/trim." << endl;
		exit(1);
	}
	int i = 1;
	bool dry_run = false;
	bool recursive = false;
	bool trim_left = true;
	bool trim_right = false;
	bool auto_confirm = false;
	bool quiet = false;
	while (argv[i][0] == '-') {
		const int arg_length = sizeof(argv[i]);
		if (arg_length > 1 && argv[i][1] != '-') {
			for (int j = 1; j < arg_length; ++j) {
				switch (argv[i][j]) {
				case 'd':
					dry_run = true;
					break;
				case 'r':
					recursive = true;
					break;
				case 'a':
					auto_confirm = true;
					break;
				case 'q':
					quiet = true;
					break;
				}
			}
		}
		else if (strcmp(argv[i], "--dry-run") == 0) {
			dry_run = true;
		}
		else if (strcmp(argv[i], "--recursive") == 0) {
			recursive = true;
		}
		else if (strcmp(argv[i], "--right") == 0) {
			trim_left = false;
			trim_right = true;
		}
		else if (strcmp(argv[i], "--both") == 0) {
			trim_left = true;
			trim_right = true;
		}
		else if (strcmp(argv[i], "--left") == 0) {
			trim_left = true;
			trim_right = false;
		}
		else if (strcmp(argv[i], "--autoconfirm") == 0) {
			auto_confirm = true;
		}
		else if (strcmp(argv[i], "--quiet") == 0) {
			quiet = true;
		}
		else {
			cerr << "Unrecognised argument '" << argv[i] << "'." << endl;
			exit(1);
		}
		++i;
	}
	if (i == argc) {
		cerr << "Please provide a prefix/postfix to replace/trim." << endl;
		exit(1);
	}

	string to_remove = argv[i];
	++i;
	string replacement = "";
	if (i < argc) {
		replacement = argv[2];
		++i;
	}

	if (!quiet) {
		std::cout << "Text to remove: '" << to_remove << "'" << endl;
		std::cout << "Substitute: '" << replacement << "'" << endl;
	}

	size_t to_remove_length = to_remove.length();
	vector<tuple<const string, const string>> toBeReplaced = {};
	auto iter = fs::directory_iterator(".");
	for (auto& entry : iter) {
		if (entry.is_regular_file()) {
			fs::path path = entry.path();
			string stem = path.filename().stem().string();
			string filename = path.filename().string();
			bool trim_left_and_prefix_found = trim_left && filename.starts_with(to_remove);
			bool trim_right_and_postfix_found = trim_right && stem.ends_with(to_remove);
			if (trim_left_and_prefix_found || trim_right_and_postfix_found)
			{
				const string before = path.string();
				if (trim_left_and_prefix_found) {
					string filename_without_prefix = filename.substr(to_remove_length);
					string new_filename = replacement + filename_without_prefix;
					filename = trim_whitespace_left(new_filename);
					path = path.parent_path().append(filename);
				}
				if (trim_right_and_postfix_found) {
					string ext = path.filename().extension().string();
					string filename_without_postfix = stem.substr(0, stem.length() - to_remove_length);
					string new_filename = filename_without_postfix + replacement;
					filename = trim_whitespace_right(new_filename) + ext;
					path = path.parent_path().append(filename);
				}

				const string after = path.string();
				std::cout << before << "  ->  " << after << endl;
				toBeReplaced.push_back({ before, after });
			}
		}
	}
	if (toBeReplaced.empty()) {
		string postfix_or_prefix_text = "prefix";
		if (trim_left && trim_right) {
			postfix_or_prefix_text = "prefix or postfix";
		}
		else if (trim_right) {
			postfix_or_prefix_text = "postfix";
		}
		std::cout << "No files with the " << postfix_or_prefix_text << " '" << to_remove << "' found." << endl;
	} else if (!dry_run) {
		std::cout << "Confirm? Y/n" << endl;
		string confirmation;
		getline(cin, confirmation);
		if (confirmation.compare("Y") == 0 || confirmation.compare("y") == 0 || confirmation.length() == 0) {
			for (tuple<const string, const string>& paths : toBeReplaced) {
				// TODO: Catch exception here when file already exists.
				std::rename(get<0>(paths).c_str(), get<1>(paths).c_str());
			}
			std::cout << "Renamed " << toBeReplaced.size() << " files." << endl;
			return 0;
		}
		else {
			std::cout << "No changes have been made." << endl;
			return 0;
		}
	}
}
