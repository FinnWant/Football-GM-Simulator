#include <string>
#include "playerH.h"
using namespace std;

// Implementations for base player
player::player(string pos, int orat, int num, int a, string n)
    :
    // initialize via member variables declared in header
    // note: base class stores are private, but implementations are in the same translation unit
    position(pos), overall_rating(orat), number(num), age(a), name(n) {}

string player::get_position() const { return position; }
int player::get_overall_rating() const { return overall_rating; }
int player::get_number() const { return number; }
int player::get_age() const { return age; }
string player::get_name() const { return name; }

void player::set_position(const string& pos) { position = pos; }
void player::set_overall_rating(int orat) { overall_rating = orat; }
void player::set_number(int num) { number = num; }
void player::set_age(int a) { age = a; }
void player::set_name(const string& n) { name = n; }

// Quarterback implementations
quarterback::quarterback(string pos, int orat, int num, int a, string n,
                         int tp, int ta, int s, int pv)
    : player(pos, orat, num, a, n), throw_power(tp), throw_accuracy(ta), speed(s), passing_vision(pv) {}

int quarterback::get_throw_power() const { return throw_power; }
int quarterback::get_throw_accuracy() const { return throw_accuracy; }
int quarterback::get_speed() const { return speed; }
int quarterback::get_passing_vision() const { return passing_vision; }

void quarterback::set_throw_power(int tp) { throw_power = tp; }
void quarterback::set_throw_accuracy(int ta) { throw_accuracy = ta; }
void quarterback::set_speed(int s) { speed = s; }
void quarterback::set_passing_vision(int pv) { passing_vision = pv; }

// Running back implementations
runningback::runningback(string pos, int orat, int num, int a, string n,
                         int s, int c, int e, int b)
    : player(pos, orat, num, a, n), speed(s), carrying(c), elusiveness(e), blocking(b) {}

int runningback::get_speed() const { return speed; }
int runningback::get_carrying() const { return carrying; }
int runningback::get_elusiveness() const { return elusiveness; }
int runningback::get_blocking() const { return blocking; }

void runningback::set_speed(int s) { speed = s; }
void runningback::set_carrying(int c) { carrying = c; }
void runningback::set_elusiveness(int e) { elusiveness = e; }
void runningback::set_blocking(int b) { blocking = b; }

// Wide receiver implementations
wide_receiver::wide_receiver(string pos, int orat, int num, int a, string n,
                             int s, int c, int rr)
    : player(pos, orat, num, a, n), speed(s), catching(c), route_running(rr) {}

int wide_receiver::get_speed() const { return speed; }
int wide_receiver::get_catching() const { return catching; }
int wide_receiver::get_route_running() const { return route_running; }

void wide_receiver::set_speed(int s) { speed = s; }
void wide_receiver::set_catching(int c) { catching = c; }
void wide_receiver::set_route_running(int rr) { route_running = rr; }

// Tight end implementations
tight_end::tight_end(string pos, int orat, int num, int a, string n,
                     int s, int c, int b)
    : player(pos, orat, num, a, n), speed(s), catching(c), blocking(b) {}

int tight_end::get_speed() const { return speed; }
int tight_end::get_catching() const { return catching; }
int tight_end::get_blocking() const { return blocking; }

void tight_end::set_speed(int s) { speed = s; }
void tight_end::set_catching(int c) { catching = c; }
void tight_end::set_blocking(int b) { blocking = b; }

// Lineman implementations
lineman::lineman(string pos, int orat, int num, int a, string n,
                 int str, int b, int t)
    : player(pos, orat, num, a, n), strength(str), blocking(b), tackling(t) {}

int lineman::get_strength() const { return strength; }
int lineman::get_blocking() const { return blocking; }
int lineman::get_tackling() const { return tackling; }

void lineman::set_strength(int s) { strength = s; }
void lineman::set_blocking(int b) { blocking = b; }
void lineman::set_tackling(int t) { tackling = t; }

// Kicker implementations
kicker::kicker(string pos, int orat, int num, int a, string n,
               int kp, int ka)
    : player(pos, orat, num, a, n), kick_power(kp), kick_accuracy(ka) {}

int kicker::get_kick_power() const { return kick_power; }
int kicker::get_kick_accuracy() const { return kick_accuracy; }

void kicker::set_kick_power(int kp) { kick_power = kp; }
void kicker::set_kick_accuracy(int ka) { kick_accuracy = ka; }

// Punter implementations
punter::punter(string pos, int orat, int num, int a, string n,
               int pp, int pa)
    : player(pos, orat, num, a, n), punt_power(pp), punt_accuracy(pa) {}

int punter::get_punt_power() const { return punt_power; }
int punter::get_punt_accuracy() const { return punt_accuracy; }

void punter::set_punt_power(int pp) { punt_power = pp; }
void punter::set_punt_accuracy(int pa) { punt_accuracy = pa; }

// Defensive back implementations
defensive_back::defensive_back(string pos, int orat, int num, int a, string n,
                               int s, int c, int t)
    : player(pos, orat, num, a, n), speed(s), coverage(c), tackling(t) {}

int defensive_back::get_speed() const { return speed; }
int defensive_back::get_coverage() const { return coverage; }
int defensive_back::get_tackling() const { return tackling; }

void defensive_back::set_speed(int s) { speed = s; }
void defensive_back::set_coverage(int c) { coverage = c; }
void defensive_back::set_tackling(int t) { tackling = t; }

// Linebacker implementations
linebacker::linebacker(string pos, int orat, int num, int a, string n,
                       int s, int t, int c)
    : player(pos, orat, num, a, n), speed(s), tackling(t), coverage(c) {}

int linebacker::get_speed() const { return speed; }
int linebacker::get_tackling() const { return tackling; }
int linebacker::get_coverage() const { return coverage; }

void linebacker::set_speed(int s) { speed = s; }
void linebacker::set_tackling(int t) { tackling = t; }
void linebacker::set_coverage(int c) { coverage = c; }

// Defensive lineman implementations
defensive_lineman::defensive_lineman(string pos, int orat, int num, int a, string n,
                                     int str, int t, int pr)
    : player(pos, orat, num, a, n), strength(str), tackling(t), pass_rushing(pr) {}

int defensive_lineman::get_strength() const { return strength; }
int defensive_lineman::get_tackling() const { return tackling; }
int defensive_lineman::get_pass_rushing() const { return pass_rushing; }

void defensive_lineman::set_strength(int s) { strength = s; }
void defensive_lineman::set_tackling(int t) { tackling = t; }
void defensive_lineman::set_pass_rushing(int pr) { pass_rushing = pr; }

