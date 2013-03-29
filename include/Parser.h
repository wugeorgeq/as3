
/* This class handles the parsing of the input .bez file
 *
 *
 *
 */

class cParser {
public:
	cParser(int, char**);
	~cParser();
	int ParseCommandLine();
		
	
private:
	int m_argc;
	char** m_argv;


};