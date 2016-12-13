
class FileHandler
{
   private:
      SymbolList * defined;
      SymbolList * undefined;

      //name mangles local symbols
      //puts undefined symbols in undefined list (if not already defined or already in undefined)
      //puts defined symbols in defined list (if not already in defined -- gives multiple definition
      //    error for multiple strong symbols) and removes from undefined if in undefined
      //updates type of symbol in defined list from weak to strong
      //name parameter: name of symbol
      //type parameter: type of symbol
      void handleObjectSymbol(std::string name, char type);

      //used to check whether an object file in an archive should be added to the
      //defined and undefined list; that is, does the object file define something
      //that is currently undefined?
      bool objectFileNeeded(std::string filename);

   public:
      FileHandler(SymbolList * defined, SymbolList * undefined);

      //returns true if the filename ends with a .a
      bool isArchive(std::string filename);

      //returns true if the filename ends with a .o
      bool isObjectFile(std::string filename);

      //handles .o files; uses nm to get the list of symbols 
      //calls handleObjectSymbol to handle each symbol
      void handleObjectFile(std::string filename);

      //handles .a files; gets the list of .o; determines whether
      //a .o should be added to the undefined and defined lists (by calling objectFileNeeded)
      //if an object file should be added, it calls handleObjectFile to do the work 
      void handleArchive(std::string filename);
};


