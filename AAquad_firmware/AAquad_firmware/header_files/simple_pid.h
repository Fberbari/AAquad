#ifndef SIMPLE_PID

#define SIMPLE_PID

void pid_set_vars(int _kp, int _kd, int _ki);

int pid_compute_x(int current_x_position, int desired_x_position);

int pid_compute_y(int current_y_position, int desired_y_position);

#endif

