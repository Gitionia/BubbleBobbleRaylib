#include "app/Application.h"

int main() {
    ApplicationParameters params {};
    // params.recordedFilePath = "./recordedInput/.input";

    Application app(params);
    app.Run();
}
