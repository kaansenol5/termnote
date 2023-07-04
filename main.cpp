#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include "include/json.hpp"




struct Note{
    std::string text;
    unsigned long long time;
};


using json = nlohmann::json;

std::string read_file(std::string filename){
    std::ifstream myfile(filename);  // open the file
    std::string contents;
    if (myfile.is_open()) {  // check if the file was opened successfully
        std::string line;
        while (getline(myfile, line)) {  // read the file line by line
            contents += line += '\n';  
        }
        myfile.close();  // close the file
    }
    else {
        return "file does not exist";
    }
    return contents;
}


int write_to_file(const std::string& text, const std::string& filename) {
    // Create an ofstream object
    std::ofstream outfile(filename);

    // Check if the file was opened
    if (!outfile) {
        std::cerr << filename << " could not be opened for writing!" << std::endl;
        return 1;
    }

    // Write to the file
    outfile << text;

    // Always close the file
    outfile.close();
    return 0;
}

void create_book(){
    system("touch book.json");
    std::string initial_state = "{\"0\":\"BOOK WAS CREATED\"}";
    write_to_file(initial_state, "book.json");
}

std::string take_note(){
    std::string command = "nvim /tmp/termnote_edit";
    system(command.c_str());
    std::string text = read_file("/tmp/termnote_edit");
    system("rm /tmp/termnote_edit");
    return text;
}

int main(int argc, char** argv){
    srand(std::time(0));
    #include <cstdlib> 
    if(argc != 2){
        std::cout << "USAGE: \n write note: termnote -w \n read notes: termnote -r" << std::endl;
        return 1;
    }
    
    if(read_file("book.json") == "file does not exist"){
        create_book();
    }
    std::string book_temp_string = read_file("book.json");
    json book = json::parse(book_temp_string);

    if(std::string(argv[1]) == "-w"){
        std::string text = take_note();
        time_t time = std::time(0);
        book += json::object_t::value_type(std::to_string(time), text);
        write_to_file(book.dump(), "book.json");
    }
    else if(std::string(argv[1]) == "-r"){
        std::string tmp_str = read_file("book.json");
        json book = json::parse(tmp_str);
        std::vector<Note> notes;
        std::cout << "Saved Notes:" << std::endl;
        unsigned i = 0;
        for(auto& element : book.items()) {
            try{
                std::string key = element.key();
                std::string value = element.value();
                unsigned long long time = std::stoi(key);
                time_t t = time;
                Note note;
                note.text = value;
                note.time = time;
                notes.push_back(note);
                std::string formatted_date = std::ctime(&t);
                std::cout << "[" << i << "] Note On: " << formatted_date << std::endl;  
                i++;
            }
            catch(nlohmann::json_abi_v3_11_2::detail::type_error){
                break;
            }

        }
        std::cout << "Which note would you like to read?" << std::endl;
        std::cout << "response: ";
        int selection;
        std::cin >> selection;
        write_to_file(notes[selection].text,"/tmp/termnote_reading");
        system("nvim /tmp/termnote_reading");
        system("rm /tmp/termnote_reading");
    }
    else{
        return 1;
    }
    return 0;
}
