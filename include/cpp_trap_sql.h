// https://xkcd.com/327/
// https://stackoverflow.com/questions/332365/how-does-the-sql-injection-from-the-bobby-tables-xkcd-comic-work
// https://en.wikipedia.org/wiki/SQL_injection

TEST_OFF
{
    using std::string;

    auto insert = [](string first_name, string last_name)
    {
        return string ("\n") + "INSERT INTO Students VALUES ('" + first_name + "', '" + last_name + "')";
    };
    TEST (insert ("Robert'); DROP TABLE Students; --", "XKCD"));
    cout << endl;

    auto authorization = [](string username, string password)
    {
        return string ("\n") + "SELECT * FROM users WHERE username='" + username + "' and (password='" + password + "')";
    };
    TEST (authorization ("Jack", "' OR '1'='1"));
    cout << endl;
};