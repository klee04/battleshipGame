#include "registerScreenState.hpp"
#include "menuScreenState.hpp"
#include "loginScreenState.hpp"

#include <iostream>

std::vector<sf::Sprite*> RegisterScreenState::sprites;
std::vector<Button*> RegisterScreenState::buttons;

RegisterScreenState::RegisterScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ) {

    // Initialize text input fields
    m_usernameField = InputField(59*4, 48*4, 102*4, 16*4, sf::Color::Black);
    m_passwordField = InputField(59*4, 88*4, 102*4, 16*4, sf::Color::Black);
    m_confirmPasswordField = InputField(59*4, 127*4, 102*4, 16*4, sf::Color::Black);

    // Initialize input field bar
    m_inputFieldBar = InputFieldBar();

    // Initialize error text
    m_errorText = *initializeText("", sf::Vector2f(182*4, 102*4), 14*4, sf::Color::Red);

    // Initialize sprites if not already initialized
    if (RegisterScreenState::sprites.empty()) {
        RegisterScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::RegisterBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (RegisterScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(180*4, 80*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::RegisterButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::RegisterButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::BackButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::BackButtonActiveTexture]),
            m_window
        ));
    }

    std::cout << "RegisterScreenState Initialized\n";
}

RegisterScreenState::~RegisterScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);
    std::cout << "RegisterScreenState Destroyed\n";
}

void RegisterScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {
                // If register button is left clicked
                if (event.mouseButton.button == sf::Mouse::Left
                && RegisterScreenState::buttons[m_buttonNames::RegisterButton]->getButtonState()) {

                    // If any input field is empty
                    if (m_usernameField.getString().empty() || m_passwordField.getString().empty() 
                    || m_confirmPasswordField.getString().empty()) {
                        playSound("invalidButtonSelect.wav");
                        m_errorText.setString("Fill in all fields!");
                        return;
                    }

                    // If password field doesn't match confirm password field
                    if (m_passwordField.getString() != m_confirmPasswordField.getString()) {
                        playSound("invalidButtonSelect.wav");
                        m_errorText.setString("Passwords don't match!");
                        return;
                    }

                    // Check if username is unique
                    if (m_stateManager.m_database->checkUsernameExists(m_usernameField.getString())) {
                        playSound("invalidButtonSelect.wav");
                        m_errorText.setString("User already exists!");
                        return;
                    }

                    // If above conditions aren't met
                    playSound("registerButtonSelect.wav");

                    // Load user data into database
                    m_stateManager.m_database->addBaseUser(
                        m_usernameField.getString(),
                        m_passwordField.getString(),
                        static_cast<IconColour>(0)
                    );
                    m_stateManager.m_database->pushToFile();

                    // Create user object 
                    m_stateManager.m_user = new User(
                        m_stateManager.m_database, 
                        m_usernameField.getString(),
                        m_passwordField.getString(),
                        static_cast<int>(m_stateManager.m_database->getUserIcon(m_usernameField.getString(), m_passwordField.getString())),
                        m_stateManager.m_database->getUserTotalWins(m_usernameField.getString(), m_passwordField.getString()),
                        m_stateManager.m_database->getUserTotalLosses(m_usernameField.getString(), m_passwordField.getString()),
                        m_stateManager.m_database->getUserTotalGames(m_usernameField.getString(), m_passwordField.getString()),
                        m_stateManager.m_database->getUserWinRate(m_usernameField.getString(), m_passwordField.getString()),
                        m_stateManager.m_database->getUserHitPercentage(m_usernameField.getString(), m_passwordField.getString())
                    );

                    std::unique_ptr<State> menuScreenState(new MenuScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(menuScreenState));

                    return;
                } else if (!m_errorText.getString().isEmpty()) 
                    m_errorText.setString("");

                // If left click on back button
                if (event.mouseButton.button == sf::Mouse::Left
                && RegisterScreenState::buttons[m_buttonNames::BackButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> loginScreenState(new LoginScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(loginScreenState));
                }
                

                // If left click on username field
                if (event.mouseButton.button == sf::Mouse::Left
                && m_usernameField.clickedWhileInactive(getMousePosition())) {
                    m_usernameField.setSelected(true);
                    m_inputFieldBar.updateInputFieldBarPosition(
                        m_usernameField.getText(), 
                        m_usernameField.getFieldRect()
                    );
                    
                    if (m_passwordField.getSelected()) m_passwordField.setSelected(false);
                    if (m_confirmPasswordField.getSelected()) m_confirmPasswordField.setSelected(false);
                    return;
                }

                // If left click on password field
                if (event.mouseButton.button == sf::Mouse::Left
                && m_passwordField.clickedWhileInactive(getMousePosition())) {
                    m_passwordField.setSelected(true);
                    m_inputFieldBar.updateInputFieldBarPosition(
                        m_passwordField.getText(), 
                        m_passwordField.getFieldRect()
                    );
                    
                    if (m_usernameField.getSelected()) m_usernameField.setSelected(false);
                    if (m_confirmPasswordField.getSelected()) m_confirmPasswordField.setSelected(false);
                    return;
                }

                // If left click on confirm password field
                if (event.mouseButton.button == sf::Mouse::Left 
                && m_confirmPasswordField.clickedWhileInactive(getMousePosition())) {
                    m_confirmPasswordField.setSelected(true);
                    m_inputFieldBar.updateInputFieldBarPosition(
                        m_confirmPasswordField.getText(), 
                        m_confirmPasswordField.getFieldRect()
                    );
                    
                    if (m_usernameField.getSelected()) m_usernameField.setSelected(false);
                    if (m_passwordField.getSelected()) m_passwordField.setSelected(false);
                    return;
                }

                // If left click on none of the input fields and one is currently selected
                if (event.mouseButton.button == sf::Mouse::Left 
                    && (m_passwordField.getSelected() || m_usernameField.getSelected() || m_confirmPasswordField.getSelected())
                    && !m_passwordField.clicked(getMousePosition())
                    && !m_usernameField.clicked(getMousePosition())
                    && !m_confirmPasswordField.clicked(getMousePosition())) {

                    if (m_passwordField.getSelected()) m_passwordField.setSelected(false);
                    if (m_usernameField.getSelected()) m_usernameField.setSelected(false);
                    if (m_confirmPasswordField.getSelected()) m_confirmPasswordField.setSelected(false);
                    m_inputFieldBar.setColour(sf::Color::Transparent);
                    return;
                }

                return;
            }

            case sf::Event::TextEntered: {
                // If neither field is selected, ignore input
                if (!m_usernameField.getSelected() && !m_passwordField.getSelected() 
                    && !m_confirmPasswordField.getSelected()) return;
                if (!m_errorText.getString().isEmpty()) m_errorText.setString("");

                // Standard characters
                if (event.text.unicode >= 33 && event.text.unicode <= 126) {
                    if (m_usernameField.getSelected() && m_usernameField.getString().length() < 19) {
                        m_usernameField.setText(m_usernameField.getString() + static_cast<char>(event.text.unicode));
                        m_inputFieldBar.updateInputFieldBarPosition(
                            m_usernameField.getText(), 
                            m_usernameField.getFieldRect()
                        );
                    }

                    if (m_passwordField.getSelected() && m_passwordField.getString().length() < 19) {
                        m_passwordField.setText(m_passwordField.getString() + static_cast<char>(event.text.unicode));
                        m_inputFieldBar.updateInputFieldBarPosition(
                            m_passwordField.getText(), 
                            m_passwordField.getFieldRect()
                            );
                    }      

                    if (m_confirmPasswordField.getSelected() && m_confirmPasswordField.getString().length() < 19) {
                        m_confirmPasswordField.setText(m_confirmPasswordField.getString() + static_cast<char>(event.text.unicode));
                        m_inputFieldBar.updateInputFieldBarPosition(
                            m_confirmPasswordField.getText(), 
                            m_confirmPasswordField.getFieldRect()
                        );
                    }   
                }

                // Backspace
                if (event.text.unicode == 8) {
                    if (m_usernameField.getSelected() && m_usernameField.getString().length() > 0) {
                        std::string copyOfString = m_usernameField.getString();
                        copyOfString.pop_back();
                        m_usernameField.setText(copyOfString);
                        m_inputFieldBar.updateInputFieldBarPosition(
                            m_usernameField.getText(), 
                            m_usernameField.getFieldRect()
                        );
                    }

                    if (m_passwordField.getSelected() && m_passwordField.getString().length() > 0) {
                        std::string copyOfString = m_passwordField.getString();
                        copyOfString.pop_back();
                        m_passwordField.setText(copyOfString);
                        m_inputFieldBar.updateInputFieldBarPosition(
                            m_passwordField.getText(), 
                            m_passwordField.getFieldRect()
                        );
                    }

                    if (m_confirmPasswordField.getSelected() && m_confirmPasswordField.getString().length() > 0) {
                        std::string copyOfString = m_confirmPasswordField.getString();
                        copyOfString.pop_back();
                        m_confirmPasswordField.setText(copyOfString);
                        m_inputFieldBar.updateInputFieldBarPosition(
                            m_confirmPasswordField.getText(), 
                            m_confirmPasswordField.getFieldRect()
                        );
                    }
                }
            

                // Enter or tab
                if (event.text.unicode == 10 || event.text.unicode == 9) {
                    if (m_confirmPasswordField.getSelected()) {
                        m_confirmPasswordField.setSelected(false);
                        m_inputFieldBar.setColour(sf::Color::Transparent);
                    }
                    
                    if (m_usernameField.getSelected()) {
                        m_usernameField.setSelected(false);
                        m_passwordField.setSelected(true);
                        m_inputFieldBar.updateInputFieldBarPosition(
                            m_passwordField.getText(), 
                            m_passwordField.getFieldRect()
                        );
                    }

                    if (m_passwordField.getSelected()) {
                        m_passwordField.setSelected(false);
                        m_confirmPasswordField.setSelected(true);
                        m_inputFieldBar.updateInputFieldBarPosition(
                            m_confirmPasswordField.getText(), 
                            m_confirmPasswordField.getFieldRect()
                        );
                    }
                }

                return;
            }

            default:
                break;
        }
    }
}

void RegisterScreenState::update() {
    sf::Vector2f mousePosition = RegisterScreenState::getMousePosition();
    buttons[m_buttonNames::RegisterButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::BackButton]->updateButtonState(mousePosition);

    if (m_usernameField.getSelected() || m_passwordField.getSelected() 
        || m_confirmPasswordField.getSelected()) m_inputFieldBar.animate();
    
}

void RegisterScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : RegisterScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : RegisterScreenState::buttons) {
        button->render();
    }

    m_usernameField.render(m_window);
    m_passwordField.render(m_window);
    m_confirmPasswordField.render(m_window);
    m_inputFieldBar.render(m_window);

    if (!m_errorText.getString().isEmpty()) {
        m_window.draw(m_errorText);
    }

    m_window.display();
}
