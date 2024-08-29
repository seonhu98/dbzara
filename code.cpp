#include <iostream>
#include <mariadb/conncpp.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

int linecount = 1;
class Search
{
private:
    std::string keyword1;
    std::string keyword2;
    std::string keyword3;
    int max = 0;
    int min = 100000;
    int sum = 0;
    int avg = 0;
    char maxname[30];
    char minname[30];
    char location[10];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    std::vector<std::string> check;
    std::vector<std::string> check2;
    std::vector<std::string> check3;

public:
    std::vector<std::string> split(std::string str, char Delimiter)
    {
        std::istringstream iss(str); // istringstream에 str을 담는다.
        std::string buffer;          // 구분자를 기준으로 절삭된 문자열이 담겨지는 버퍼
        std::vector<std::string> result;

        // istringstream은 istream을 상속받으므로 getline을 사용할 수 있다.
        while (getline(iss, buffer, Delimiter))
        {
            result.push_back(buffer); // 절삭된 문자열을 vector에 저장
        }
        return result;
    }
    void history()
    {
        int countnum;
        std::ifstream file;
        std::string buffer = " ";
        file.open("history.txt", std::ios_base::out);
        getline(file, buffer);
        while (getline(file, buffer))
        {
            std::cout << buffer << std::endl;
        }
    }

    void reserch()
    {
        std::ifstream in("history.txt", std::ios_base::out);
        std::string s;
        in.seekg(0, std::ios::end); // 위치 지정자를 파일 끝으로 옮긴다.
        int size = in.tellg();      // 그리고 그 위치를 읽는다. (파일의 크기)
        s.resize(size);             // 그 크기의 문자열을 할당한다.
        in.seekg(0, std::ios::beg); // 위치 지정자를 다시 파일 맨 앞으로 옮긴다.
        in.read(&s[0], size);
        in.close();
        in.open("histort.txt", std::ios_base::in);
        std::string buffer;
        std::getline(in, buffer);
        std::getline(in, buffer);
        std::string a;
        std::cout << "재검색할 내용 : ";
        std::cin >> a;
        std::cout << s.substr(s.find(a)) << std::endl;
        std::string spstr = s.substr(s.find(a));
        int ia = stoi(a);
        std::vector<std::string> splitstring;
        splitstring = split(spstr, ' ');
        std::vector<std::string>str_1;
        std::vector<std::string>str_2;
        std::vector<std::string>str_3;
        str_1.push_back(splitstring[ia+6]);
        str_2.push_back(splitstring[ia+7]);
        str_3.push_back(splitstring[ia+8]);
        std::cout <<str_1[0]<<str_2[0]<<str_3[0]<<std::endl;
        in.close();
    }

    void showParks(std::unique_ptr<sql::Connection> &conn)
    {
        try
        {
            // createStaemet 객체 생성
            std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
            // 쿼리를 실행
            int i = 1;
            sql::ResultSet *res = stmnt->executeQuery("select * from PARK");
            // 반복문을 통해서 내부의 값을 반환
            while (res->next())
            {
                if (keyword2.compare(res->getString(2)) == 0 && keyword3.compare(res->getString(3)) == 0)
                {
                    std::cout << "지역구: " << res->getString(2);
                    std::cout << ", 법정동: " << res->getString(3);
                    std::cout << i++ << " : " << res->getString(4) << std::endl;
                }
                else
                    continue;
            }
            // 실패시 오류 메세지 반환
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "Error selecting tasks: " << e.what() << std::endl;
        }
    }

    void showStats(std::unique_ptr<sql::Connection> &conn)
    {
        try
        {
            int i = 1;
            std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
            sql::ResultSet *res = stmnt->executeQuery("select * from STATS");
            while (res->next())
            {
                if (keyword2.compare(res->getString(2)) == 0 && keyword3.compare(res->getString(3)) == 0)
                {
                    std::cout << i++ << " : " << res->getString(7);
                    std::cout << "\n";
                }
                else
                    continue;
            }
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "Error selecting tasks: " << e.what() << std::endl;
        }
    }

    void showMarts(std::unique_ptr<sql::Connection> &conn)
    {
        try
        {
            int i = 1;
            std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
            sql::ResultSet *res = stmnt->executeQuery("select* from MART");
            while (res->next())
            {

                if (keyword2.compare(res->getString(2)) == 0 && keyword3.compare(res->getString(3)) == 0)
                {
                    std::cout << i++;
                    std::cout << " : " << res->getString(4);
                    std::cout << " ( " << res->getString(5) << " )" << std::endl;
                }
                else
                    continue;
            }
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "Error selecting tasks: " << e.what() << std::endl;
        }
    }

    void ShowRealestate(std::unique_ptr<sql::Connection> &conn)
    {
        try
        {
            std::cout << "검색 가능한 주거 형태는 다음과 같습니다." << std::endl;
            std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
            sql::ResultSet *res = stmnt->executeQuery("select * from BUILDINGS");
            while (res->next())
            {
                if (find(check.begin(), check.end(), res->getString(2)) == check.end())
                    check.emplace_back(res->getString(2));
            }

            std::cout << "[ 조회 원하시는 것을 입력 부탁드립니다 ]" << std::endl;
            std::cout << "========================================================================================" << std::endl;
            for (int i = 0; i < check.size(); i++)
            {
                std::cout << check.at(i) << ' ' << std::endl;
                if (i % 10 == 0)
                {
                    std::cout << std::endl;
                }
            }
            std::cout << "========================================================================================" << std::endl;
            std::cout << " 입력 : ";
            std::cin >> keyword1;
            res = stmnt->executeQuery("select * from BUILDINGS");
            int i = 0;
            while (res->next())
            {
                if (res->getString(2) == keyword1)
                {
                    if (find(check2.begin(), check2.end(), res->getString(3)) == check2.end())
                        check2.emplace_back(res->getString(3));
                }
            }
            std::cout << "[ 조회 원하시는 것을 입력 부탁드립니다 ]" << std::endl;
            std::cout << "========================================================================================" << std::endl;
            for (i; i < check2.size(); i++)
            {
                std::cout << check2.at(i) << ' ';
                if ((i + 1) % 10 == 0)
                {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl
                      << "========================================================================================" << std::endl;
            std::cout << " 입력 : ";
            std::cin >> keyword2;
            res = stmnt->executeQuery("select * from BUILDINGS");
            while (res->next())
            {
                if (res->getString(2) == keyword1 && res->getString(3) == keyword2)
                {
                    if (find(check3.begin(), check3.end(), res->getString(4)) == check3.end())
                        check3.emplace_back(res->getString(4));
                }
            }
            std::cout << "[ 조회 원하시는 것을 입력 부탁드립니다 ]" << std::endl;
            std::cout << "========================================================================================" << std::endl;
            for (int i = 0; i < check3.size(); i++)
            {
                std::cout << check3.at(i) << ' ';
                if ((i + 1) % 10 == 0)
                {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl
                      << "========================================================================================" << std::endl;
            std::cout << " 입력 : ";
            std::cin >> keyword3;
    
            res = stmnt->executeQuery("select * from BUILDINGS");
            std::cout << "입력하신 지역의 (" << keyword1 << ") 최근 거래 정보입니다." << std::endl;
            std::cout << "[ 지역구: " << keyword2;
            std::cout << "/ 법정동: " << keyword3 << " ]" << std::endl;
            std::cout << "========================================================================================" << std::endl;
            std::ofstream addfile;
            addfile.open("history.txt", std::ios::app);
            addfile << linecount << ". " << tm.tm_year + 1900 << "년 " << tm.tm_mon + 1 << "월 " << tm.tm_mday << "일 "
                    << tm.tm_hour << "시 " << tm.tm_min << "분 " << ": " << keyword1 << " " << keyword2 << " " << keyword3 << std::endl;
            linecount++;
            addfile.close();

            while (res->next())
            {
                if (keyword1.compare(res->getString(2)) == 0 && keyword2.compare(res->getString(3)) == 0 && keyword3.compare(res->getString(4)) == 0)
                {
                    std::cout << i++;
                    std::cout << " : " << res->getString(7) << " " << res->getInt(11) << " 층";
                    std::cout << " [ " << res->getInt(9) << " 만원 / " << res->getInt(12) << "년 준공 ]" << std::endl;
                    strcpy(location, res->getString(4));
                    if (max < res->getInt(9) && res->getInt(9) != 0)
                    {
                        max = res->getInt(9);
                        strcpy(maxname, res->getString(7));
                    }
                    if ((min > res->getInt(9) && res->getInt(9) != 0))
                    {
                        min = res->getInt(9);
                        strcpy(minname, res->getString(7));
                    }
                    sum += res->getInt(9);
                }
            }
            std::cout << "총 매물 :" << i - 1 << "개" << std::endl;
            std::cout << "최고 가격은 " << maxname << " " << max << "(만원) 입니다" << std::endl;
            std::cout << "최저 가격은 " << minname << " " << min << "(만원) 입니다" << std::endl;
            avg = sum / i - 1;
            std::cout << location << "의 매물 평균 가격은 " << avg << "(만원) 입니다" << std::endl;
            std::cout << "========================================================================================" << std::endl;
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "Error selecting tasks: " << e.what() << std::endl;
        }
    }

    void
    showSubway(std::unique_ptr<sql::Connection> &conn)
    {
        try
        {
            int i = 1;
            std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
            sql::ResultSet *res = stmnt->executeQuery("select * from SUB");
            while (res->next())
            {
                if (keyword2.compare(res->getString(6)) == 0 && keyword3.compare(res->getString(8)) == 0)
                {
                    i++;
                    std::cout << " : " << res->getInt(3) << "호선 [" << res->getString(4) << "역]" << std::endl;
                    std::cout << res->getString(8) << "지역의 지하철 수: " << i - 1 << "대" << std::endl;
                    if (i - 1 > 1)
                    {
                        std::cout << "환승이 가능합니다" << std::endl;
                    }
                }
                else
                    continue;
            }
        }

        catch (sql::SQLException &e)
        {
            std::cerr << "Error selecting tasks: " << e.what() << std::endl;
        }
    }
    void Strat(std::unique_ptr<sql::Connection> &conn)
    {
        ShowRealestate(conn);
        int count = 0;
        char sel;
        std::cout << " 매물 인근의 편의 시설을 검색할 수 있습니다." << std::endl
                  << "원하는 편의 시설을 선택해 주세요." << std::endl;
        std::cout << "0 : 처음으로\t1 : 부동산\t2 : 지하철\n3 : 마트\t4 : 공원\tQ : 종료\nH : 히스토리보기\tR : 재검색" << std::endl;
        std::cin >> sel;
        // getchar();
        while (count < 1)
        {
            switch (sel)
            {
            case '0':
                count++;
                break;

            case '1':
                system("clear");
                showStats(conn);
                count++;
                break;

            case '2':
                system("clear");
                showSubway(conn);
                count++;
                break;

            case '3':
                system("clear");
                showMarts(conn);
                count++;
                break;

            case '4':
                system("clear");
                showParks(conn);
                count++;
                break;

            case 'q':
            case 'Q':
                std::cout << "<system : 프로그램을 종료하겠습니다>";
                exit(1);
                break;

            case 'h':
            case 'H':
                history();
                count++;
                break;

            case 'r':
            case 'R':
                history();
                reserch();
                count++;
                break;
            }
        }
    }
};

int main()
{
    try
    {
        Search Searching;
        sql::Driver *driver = sql::mariadb::get_driver_instance();
        sql::SQLString url("jdbc:mariadb://10.10.21.123/DBZARE");
        sql::Properties properties({{"user", "sh"}, {"password", "1234"}});
        std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

        while (1)
            Searching.Strat(conn);
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error Connecting to MariaDB Platform: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
