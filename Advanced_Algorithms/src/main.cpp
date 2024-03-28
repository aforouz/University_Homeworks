#include "Main.hpp"

Model init_model(Config& _config, Matrix _matrix)
{
    _matrix.delete_negative();
    if (_matrix.row() > _matrix.column())
    {
        _matrix.transpose();
    }
    return Model(_config.k(), _config.method(), _matrix);
}

int main(int argc, char *argv[])
{
    Config config;
    Matrix matrix;
    Model model;
    string directory_address;
    
    config = config_file();
    matrix = scan_data(config.directory_address());
    model = init_model(config, matrix);
    model.set_start_time();
    menu(config, model);
    model.set_end_time();
    print_data(model, config.directory_address());

    return EXIT_SUCCESS; // exit(EXIT_SUCCESS);
}