//
// Created by Pierre Juliot on 11/21/2020.
//

#include "GlobalVariables.h"


const string badTimeAnswers[] = {"Wake up!",
                                "You must be at least 90 years old!",
                                "Are you asleep?",
                                "My dog would have done better than this!",
                                "Ask someone else to do it for you!",
                                "Meh...",
                                "You're doing it on purpose, right?",
                                "It's amazingly bad...",
                                "You have to react as fast as possible. Saying that just in case..."};

const string okTimeAnswers[] = { "OK!",
                                "Good enough!",
                                "Nice attempt!",
                                "You're on the right path!",
                                "Practice makes perfect!",
                                "Average!",
                                "Keep going!",
                                "Keep focusing!",
                                "You're close to perfection!"};

const string goodTimeAnswers[] = {"Amazing!",
                                 "Incredible!",
                                 "Are you a robot?",
                                 "GG!",
                                 "No way! You must be cheating right?",
                                 "*clap* *clap* *clap*",
                                 "I think you're a professional.",
                                 "Pretty good!",
                                 "Faster than light!"};

const string reactionTimeTitle = "    ____                  __  _                _______                __\n"
                                "   / __ \\___  ____ ______/ /_(_)___  ____     /_  __(_)___ ___  ___  / /\n"
                                "  / /_/ / _ \\/ __ `/ ___/ __/ / __ \\/ __ \\     / / / / __ `__ \\/ _ \\/ / \n"
                                " / _, _/  __/ /_/ / /__/ /_/ / /_/ / / / /    / / / / / / / / /  __/_/  \n"
                                "/_/ |_|\\___/\\__,_/\\___/\\__/_/\\____/_/ /_/    /_/ /_/_/ /_/ /_/\\___(_)   \n";

const string getReadyTitle = "   ______     __     ____                 __      __\n"
                            "  / ____/__  / /_   / __ \\___  ____  ____/ /_  __/ /\n"
                            " / / __/ _ \\/ __/  / /_/ / _ \\/ __ `/ __  / / / / / \n"
                            "/ /_/ /  __/ /_   / _, _/  __/ /_/ / /_/ / /_/ /_/  \n"
                            "\\____/\\___/\\__/  /_/ |_|\\___/\\__,_/\\__,_/\\__, (_)   \n"
                            "                                        /____/      ";

const string nowTitle = "    _   __              __\n"
                        "   / | / /___ _      __/ /\n"
                        "  /  |/ / __ \\ | /| / / / \n"
                        " / /|  / /_/ / |/ |/ /_/  \n"
                        "/_/ |_/\\____/|__/|__(_)   \n"
                        "                          ";

const string resultsTitle = "    ____                  ____      \n"
                            "   / __ \\___  _______  __/ / /______\n"
                            "  / /_/ / _ \\/ ___/ / / / / __/ ___/\n"
                            " / _, _/  __(__  ) /_/ / / /_(__  ) \n"
                            "/_/ |_|\\___/____/\\__,_/_/\\__/____/  \n";

const string multipleAttemptsTitle = "    __  ___      ____  _       __        ___   __  __                       __      \n"
                                     "   /  |/  /_  __/ / /_(_)___  / /__     /   | / /_/ /____  ____ ___  ____  / /______\n"
                                     "  / /|_/ / / / / / __/ / __ \\/ / _ \\   / /| |/ __/ __/ _ \\/ __ `__ \\/ __ \\/ __/ ___/\n"
                                     " / /  / / /_/ / / /_/ / /_/ / /  __/  / ___ / /_/ /_/  __/ / / / / / /_/ / /_(__  ) \n"
                                     "/_/  /_/\\__,_/_/\\__/_/ .___/_/\\___/  /_/  |_\\__/\\__/\\___/_/ /_/ /_/ .___/\\__/____/  \n"
                                     "                    /_/                                          /_/                ";

bool timerCheat = false;