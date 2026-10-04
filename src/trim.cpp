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

#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

string trim_whitespace_left(string input) {
	return input.erase(0, input.find_first_not_of(' '));
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		cout << "Please provide a prefix to replace/trim." << endl;
		exit(1);
	}
	int i = 1;
	bool dry_run = false;
	while (strcmp(argv[i], "-d") == 0) {
		dry_run = true;
		++i;
		if (i == argc) {
			cout << "Please provide a prefix to replace/trim." << endl;
			exit(1);
		}
	}

	string prefix = argv[i];
	++i;
	string replacement = "";
	if (i < argc) {
		replacement = argv[2];
		++i;
	}

	cout << "Prefix: '" << prefix << "'" << endl;
	cout << "Replacement: '" << replacement << "'" << endl;

	size_t prefix_length = prefix.length();
	vector<tuple<const string, const string>> toBeReplaced = {};
	auto iter = fs::directory_iterator(".");
	for (auto& entry : iter) {
		if (entry.is_regular_file() && entry.path().filename().string().starts_with(prefix)) {
			const fs::path path = entry.path();
			string filename_without_prefix = path.filename().string().substr(prefix_length);
			string new_filename = replacement + filename_without_prefix;
			string left_trimmed = trim_whitespace_left(new_filename);
			fs::path new_path = path.parent_path().append(left_trimmed);

			const string before = path.string();
			const string after = new_path.string();
			cout << before << "  ->  " << after << endl;

			toBeReplaced.push_back({ before, after });
		}
	}
	if (toBeReplaced.empty()) {
		cout << "No files with the prefix '" << prefix << "' found." << endl;
	} else if (!dry_run) {
		cout << "Confirm? Y/n" << endl;
		string confirmation;
		getline(cin, confirmation);
		if (confirmation.compare("Y") == 0 || confirmation.compare("y") == 0 || confirmation.length() == 0) {
			for (tuple<const string, const string>& paths : toBeReplaced) {
				// TODO: Catch exception here when file already exists.
				std::rename(get<0>(paths).c_str(), get<1>(paths).c_str());
			}
			cout << "Renamed " << toBeReplaced.size() << " files." << endl;
			return 0;
		}
		else {
			cout << "No changes have been made." << endl;
			return 0;
		}
	}
}
