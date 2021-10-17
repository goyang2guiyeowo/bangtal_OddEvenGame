#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <bangtal>
using namespace bangtal;
#include <cstdlib>
#include <time.h>

ScenePtr scene;
ObjectPtr start, explain, ch[4], fist, odd, even, palm, marble[4], yes, no, next;
int myM, fistX = -500, yourM = 4, myTotal = 4, yourTotal = 4, chance = 2, count = 0, option;
bool myGuess;
char message[50], marbleChar[20];

TimerPtr timer;
float pauseTime = 0.03f;
int moveCount = 30;

//찬스 사용
void use_chance() {
    chance--;

    showMessage("상대 : 자네가. . 방금 뭐라고. . 했지. . ?");
    odd->show();
    even->show();
}

//정답이면
void correct() {
    if (yourM <= myM) {
        myTotal += yourM;
        yourTotal -= yourM;
    }
    else {
        myTotal += myM;
        yourTotal -= myM;
    }

    if (myTotal >= 8) {
        sprintf(message, "이겼다!!\n내 구슬 : 8개 / 상대 구슬 : 0개\n총 %d판", count);
        showMessage(message); //상대의 구슬을 다 뺏었을 경우 승리
    }
    else {
        sprintf(message, "정답!\n내 구슬 : %d개 / 상대 구슬 : %d개\n남은 찬스 : %d회", myTotal, yourTotal, chance);
        showMessage(message);
        next->show();
    }
}

//오답이면
void wrong() {
    //찬스가 없으면
    if (chance <= 0) {
        if (yourM <= myM) {
            myTotal -= yourM;
            yourTotal += yourM;
        }
        else {
            myTotal -= myM;
            yourTotal += myM;
        }

        if (myTotal <= 0) {
            sprintf(message, "졌다!!ㅠㅠ\n내 구슬 : 0개 / 상대 구슬 : 8개\n총 %d판", count);
            showMessage(message); //상대에게 구슬을 다 뺏겼을 경우 패배
        }
        else {
            sprintf(message, "오답!\n내 구슬 : %d개 / 상대 구슬 : %d개\n남은 찬스 : %d회", myTotal, yourTotal, chance);
            showMessage(message);
            next->show();
        }
    }
    //찬스가 있으면
    else {
        sprintf(message, "오답!\n남은 찬스 : %d회\n찬스를 쓸까?", chance);
        showMessage(message);

        //응 아니 선택
        yes = Object::create("yes.jpg", scene, 750, 300);
        no = Object::create("no.jpg", scene, 930, 300);

        yes->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
            yes->hide();
            no->hide();

            use_chance();

            return true;
            });

        no->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
            yes->hide();
            no->hide();

            if (yourM <= myM) {
                myTotal -= yourM;
                yourTotal += yourM;
            }
            else {
                myTotal -= myM;
                yourTotal += myM;
            }

            if (myTotal <= 0) {
                sprintf(message, "졌다!!ㅠㅠ\n내 구슬 : 0개 / 상대 구슬 : 8개\n총 %d판", count);
                showMessage(message); //상대에게 구슬을 다 뺏겼을 경우 패배
            }
            else {
                sprintf(message, "내 구슬 : %d개 / 상대 구슬 : %d개\n남은 찬스 : %d회", myTotal, yourTotal, chance);
                showMessage(message);
                next->show();
            }
       
            return true;
            });
    }
}

//정답 공개
void open_fist() {
    odd->hide();
    even->hide();
    fist->hide();
    palm->show();

    for (int i = 0; i < yourM; i++) {
        marble[i]->show();
    }

    if ((yourM % 2) == myGuess) correct();
    else wrong();
}

//구슬을 몇개 걸지 선택
void choose_num() {
    palm->hide();

    for (int i = 0; i < yourM; i++) {
        marble[i]->hide();
    }

    showMessage("구슬을 몇 개 걸까?");

    option = (myTotal > 4) ? 4 : myTotal;
    for (int i = 0; i < option; i++) {
        ch[i]->show();
    }

    count++;
}

//상대 구슬 홀짝 맞히기
void guess() {
    //주먹 이동
    fistX = -500;
    moveCount = 30;
    fist->show();
    fist->locate(scene, fistX, -200);
    timer->set(pauseTime);
    timer->start();

    //상대 구슬 개수 결정
    yourM = rand() % 4 + 1;

    //홀짝 버튼 생성
    odd = Object::create("odd.png", scene, 700, 250);
    even = Object::create("even.png", scene, 900, 250);

    odd->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        myGuess = true;
        open_fist();

        return true;
        });

    even->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        myGuess = false;
        open_fist();

        return true;
        });
}

//게임 초기화
void init_game() {
    //불필요한 기본옵션 제거
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

    //배경 생성
    scene = Scene::create("홀짝 구슬게임", "background.jpg");

    //구슬 개수 선택 버튼 생성
    char path[10];
    for (int i = 0; i < 4; i++) {
        sprintf(path, "ch%d.png", i + 1);
        ch[i] = Object::create(path, scene, 100 + 250 * i, 450 + 10 * i);
        ch[i]->hide();
    }
    ch[0]->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        myM = 1;
        for (int i = 0; i < 4; i++) {
            ch[i]->hide();
        }
        guess();

        return true;
        });
    ch[1]->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        myM = 2;
        for (int i = 0; i < 4; i++) {
            ch[i]->hide();
        }
        guess();

        return true;
        });
    ch[2]->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        myM = 3;
        for (int i = 0; i < 4; i++) {
            ch[i]->hide();
        }
        guess();

        return true;
        });
    ch[3]->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        myM = 4;
        for (int i = 0; i < 4; i++) {
            ch[i]->hide();
        }
        guess();

        return true;
        });

    //주먹 생성
    fist = Object::create("fist.png", scene, fistX, -200);
    fist->hide();

    //주먹 이동시키는 타이머 생성
    timer = Timer::create(pauseTime);
    timer->setOnTimerCallback([&](auto)->bool {
        fistX = fistX + 8;
        fist->locate(scene, fistX, -200);

        moveCount--;
        if (moveCount > 0) {
            timer->set(pauseTime);
            timer->start();
        }

        return true;
        });

    //손바닥 생성
    palm = Object::create("palm.png", scene, 0, 50);
    palm->hide();

    //손에 구슬 생성
    for (int i = 0; i < 4; i++) {
        marble[i] = Object::create("marble.png", scene, 130 + 70 * i, 300);
        marble[i]->hide();
    }

    //다음판 버튼 생성
    next = Object::create("next.jpg", scene, 1000, 600);
    next->hide();
    next->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        next->hide();
        
        choose_num();

        return true;
        });

    //시작버튼 생성
    start = Object::create("start.png", scene, 440, 200);
    start->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        start->hide();
        explain->show();

        return true;
        });

    //게임설명창 생성
    explain = Object::create("explain.png", scene, 270, 30);
    explain->hide();
    explain->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        explain->hide();
        choose_num();

        return true;
        });

    startGame(scene);
}

int main()
{
    srand((unsigned int)time(NULL));

    init_game();
}