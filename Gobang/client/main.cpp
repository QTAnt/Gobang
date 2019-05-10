#include "game.hpp"

static void Usage(string proc) //协议
{
    cout << "Usage: "<< proc << " ip port " << endl;    
}

static void LoginMenu()
{
    cout << "##########################################" << endl;
    cout << "#####   1.登录          2. 注册      #####" << endl;
    cout << "#####   3.退出                       #####" << endl;
    cout << "##########################################" << endl;
    cout << "Please Select:> ";
}

int main(int argc, char *argv[])
{
    if(argc != 3){
        Usage(argv[0]);
        return 1;
    }
    string ip = argv[1];
    int port = atoi(argv[2]);

    string name;
    string passwd;
    uint32_t id = 0;
    int select = 0;
    while(1){
        LoginMenu();
        cin >> select;
        switch(select){
            case 1:
                {
                    uint32_t result = Login(ip, port);
                    if(result >= 10000){
                        id = result;
                        cout << "登录成功，开始匹配吧^_^ !" << endl;
                        Game(ip, port, id);
                        //Game(id);
                    }else{
                        cout << "登录失败，退出码：" << result << endl;
                    } //1.未注册 2.验证失败 3.超时
                }
                break;
            case 2:
                Register(ip, port, name, passwd, id);
                cout << "注册成功，请务必记住你的登录ID：[" << id << "]" << endl;
                cout << "请直接登录！" << endl;
                break;
            case 3:
                cout << "Client Quit!" << endl;
                exit(2);
            default:
                cout << "Enter Error ! Try Again!" << endl;
                break;
        }
    }
}


