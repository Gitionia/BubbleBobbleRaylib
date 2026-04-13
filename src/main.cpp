#include "app/Application.h"
#include "utils/Log.h"
#include "utils/Utilities.h"
#include <string>

static void printHelp() {
    printf("\nPossible args are: \n\n"
           "--levelFile <LevelXYZ.json>                 starts at level XYZ, located in res/levels/\n"
           "--levelNr <levelNr>                         starts at level levelNr\n"
           "--playRecording path/to/recording.input     plays the game with the recording.input file \n"
           "--help                                      displays help message \n\n\n");
}


int main(int argc, char **argv) {
    ApplicationParameters params = ParseApplicationParametersAndPrintErrors(argc, argv);
    // params.recordedFilePath = "./recordedInput/.input";

    if (params.printHelp) {
        printHelp();
    }

    if (params.runApp) {
        Application app(params);
        app.Run();
    }
}
