# Bomb-o-woman Game

This repository contains a C++ program developed as an assignment for the "Programmieren 2" course in the Technical Computer Science program at the Augsburg University of Applied Sciences. The goal of the project is to learn and apply the concepts of object-oriented programming (OOP), such as classes and instances interaction, data encapsulation, and inheritance. The game is designed for 2-4 players, where players collect bombs and power-ups, destroy rocks and opponents for points, and the game ends when a player has no lives left. The player with the highest points wins.

## Project Structure

The project consists of several files and classes:

- `Configuration`: Contains the namespace for game configuration constants.
- `Position`: Defines the class representing positions on the game board.
- `GameObject`: Defines the base class for all game objects, including players, bombs, power-ups, and obstacles.
- `Player`: Defines the class for player characters, including their actions and movement.
- `Input` : Defines the class for input handling.

## Learning Objectives

The main learning objectives of this project include:

- Understanding the fundamentals of object-oriented programming (OOP).
- Implementing classes and instances interaction.
- Applying data encapsulation and inheritance concepts.
- Developing a simple multiplayer game in C++.

## Compilation and Execution

To compile and run the program, follow these steps:

1. Make sure you have a C++ compiler installed (e.g., g++).
2. Open a terminal and navigate to the directory containing the source files.
3. Compile the program using the following command:

    ``` bash
    g++ -o bomb_o_woman ./src/*.cpp
    ```

4. Run the compiled program with:

    ``` bash
    ./bomb_o_woman
    ```

## Contributing

Contributions to this project are welcome. If you have any suggestions for improvements or additional features, please feel free to open an issue or submit a pull request on the project's GitHub repository.

## License

This project is licensed under the Apache 2.0 License. See the LICENSE file for more information.

## Acknowledgments

We would like to thank the instructors and the teaching assistants of the "Programmieren 2" course for their guidance and support throughout the development of this project.

Additionally, we appreciate the contributions of fellow students and open source community members who have provided feedback and suggestions for improvement.

## Disclaimer

This project is meant for educational purposes only. The creators of this project are not responsible for any misuse or consequences resulting from the use of the program.
