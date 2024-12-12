#include "modifiedLoginScreenState.hpp"
#include "passAndPlayFleetDeploymentScreenState.hpp"
#include "playerTwoLoginScreenState.hpp"

#include <iostream>

std::vector<sf::Sprite*> ModifiedLoginScreenState::sprites;
std::vector<Button*> ModifiedLoginScreenState::buttons;

ModifiedLoginScreenState::ModifiedLoginScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ) {

    // Initialize text input fields
    m_usernameField = InputField(109*4, 52*4, 102*4, 16*4, sf::Color::Black);
    m_passwordField = InputField(109*4, 92*4, 102*4, 16*4, sf::Color::Black);

    // Initialize input field bar
    m_inputFieldBar = InputFieldBar();

    // Initialize error text
    m_errorText = initializeText("", sf::Vector2f(122*4, 142*4), 14*4, sf::Color::Red);

    // Initialize sprites if not already initialized
    if (ModifiedLoginScreenState::sprites.empty()) {
        ModifiedLoginScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::ModifiedLoginBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (ModifiedLoginScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(120*4, 120*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LoginButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LoginButtonActiveTexture]),
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

    std::cout << "ModifiedLoginScreenState Initialized\n";
}

ModifiedLoginScreenState::~ModifiedLoginScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);

    delete m_errorText;
    m_errorText = nullptr;

    std::cout << "ModifiedLoginScreenState Destroyed\n";
}

void ModifiedLoginScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;

            case sf::Event::MouseButtonReleased: {

                // If back button is left clicked
                if (event.mouseButton.button == sf::Mouse::Left
                 && ModifiedLoginScreenState::buttons[m_buttonNames::BackButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> playerTwoLoginScreenState(new PlayerTwoLoginScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(playerTwoLoginScreenState));
                    return;
                }

                // If login button is left clicked
                if (event.mouseButton.button == sf::Mouse::Left
                 && ModifiedLoginScreenState::buttons[m_buttonNames::LoginButton]->getButtonState()) {

                    // If any input field is empty
                    if (m_usernameField.getString().empty() || m_passwordField.getString().empty()) {
                        playSound("invalidButtonSelect.wav");
                        m_errorText->setString("Fill in all fields!");
                        return;
                    }

                    // If username and password don't match any users in database
                    if (!m_stateManager.m_database->checkUser(m_usernameField.getString(), m_passwordField.getString())) {
                        playSound("invalidButtonSelect.wav");
                        m_errorText->setString("Incorrect credentials!");
                        return;
                    }

                    // If user is already logged in as Player 1
                    if (!m_stateManager.m_user->getIsGuest() &&
                        m_usernameField.getString() == m_stateManager.m_user->getUsername()) {
                        playSound("invalidButtonSelect.wav");
                        m_errorText->setString("User already logged in!");
                        return;
                    }

                    // If above conditions are not met
                    playSound("buttonSelect.wav");

                    // Create user object for 2nd User
                    m_stateManager.m_secondUser = new User(
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

                    // Proceed to fleet deployment
                    int emptyArray[12][12] = {};
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> passAndPlayFleetDeploymentScreenState(new PassAndPlayFleetDeploymentScreenState(
                        m_stateManager, m_window, emptyArray, false
                    ));
                    m_stateManager.changeState(std::move(passAndPlayFleetDeploymentScreenState));

                    return;
                } else if (!m_errorText->getString().isEmpty()) 
                    m_errorText->setString("");

                // If left click on username field
                if (event.mouseButton.button == sf::Mouse::Left
                && m_usernameField.clickedWhileInactive(getMousePosition())) {
                    m_usernameField.setSelected(true);
                    m_inputFieldBar.updateInputFieldBarPosition(
                        m_usernameField.getText(), 
                        m_usernameField.getFieldRect()
                    );
                    
                    if (m_passwordField.getSelected()) m_passwordField.setSelected(false);
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
                    return;
                }

                // If left click on neither input field and one is currently selected
                if (event.mouseButton.button == sf::Mouse::Left 
                    && (m_passwordField.getSelected() || m_usernameField.getSelected())
                    && !m_passwordField.clicked(getMousePosition())
                    && !m_usernameField.clicked(getMousePosition())) {

                    if (m_passwordField.getSelected()) m_passwordField.setSelected(false);
                    if (m_usernameField.getSelected()) m_usernameField.setSelected(false);
                    m_inputFieldBar.setColour(sf::Color::Transparent);
                    return;
                }
                
                return;
            }

            case sf::Event::TextEntered: {
                // If neither field is selected, ignore input
                if (!m_usernameField.getSelected() && !m_passwordField.getSelected()) return;
                if (!m_errorText->getString().isEmpty()) m_errorText->setString("");

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
                }
            

                // Enter or tab
                if (event.text.unicode == 10 || event.text.unicode == 9) {
                    if (m_passwordField.getSelected()) {
                        m_passwordField.setSelected(false);
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
                }

                return;
            }

            default:
                break;
        }
    }
}

void ModifiedLoginScreenState::update() {
    sf::Vector2f mousePosition = ModifiedLoginScreenState::getMousePosition();
    buttons[m_buttonNames::LoginButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::BackButton]->updateButtonState(mousePosition);

    if (m_usernameField.getSelected() || m_passwordField.getSelected()) {
        m_inputFieldBar.animate();
    }

}

void ModifiedLoginScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : ModifiedLoginScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : ModifiedLoginScreenState::buttons) {
        button->render();
    }

    m_usernameField.render(m_window);
    m_passwordField.render(m_window);
    m_inputFieldBar.render(m_window);

    if (!m_errorText->getString().isEmpty()) {
        m_window.draw(*m_errorText);
    }

    m_window.display();
}
