#include "Stand.h"

/******************************************************************************/
struct Stand {
    char *name;
    unsigned char *seats;
    unsigned int num_seats;
};

/******************************************************************************/
unsigned int get_num_seats_in_bytes(unsigned int num_seats) {
    unsigned int num_seats_in_bytes = num_seats / BYTE_SIZE;
    // if the number of seats is not a multiple of 8, add one byte to the number
    if (num_seats % BYTE_SIZE != 0) {
        num_seats_in_bytes++;
    }
    return num_seats_in_bytes;
}

/******************************************************************************/
Stand StandCreate(const char *name, unsigned int num_seats) {
    Stand new_stand = (Stand) malloc(sizeof(struct Stand));
    if (!new_stand) {
        fprintf(stderr, "Failed To Allocate Memory!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }

    new_stand->name = (char *) malloc((strlen(name) + 1) * sizeof(char));
    if (!new_stand->name) {
        free(new_stand);
        fprintf(stderr, "Failed To Allocate Memory!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }
    strcpy(new_stand->name, name);

    unsigned int num_seats_in_bytes = get_num_seats_in_bytes(num_seats);
    new_stand->seats = calloc(num_seats_in_bytes, sizeof(char));
    if (!new_stand->seats) {
        free(new_stand->name);
        free(new_stand);
        fprintf(stderr, "Failed To Allocate Memory!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }

    new_stand->num_seats = num_seats;

    return new_stand;

}

/******************************************************************************/
void StandDestroy(Stand stand) {
    free(stand->name);
    free(stand->seats);
    free(stand);
}

/******************************************************************************/
unsigned int StandMaxCapacity(Stand stand) {
    return stand->num_seats;
}

/******************************************************************************/
const char *StandName(Stand stand) {
    return stand->name;
}

/******************************************************************************/
unsigned int count_zero_bits(unsigned char byte) {
    int i;
    unsigned int zero_bits = 0;
    // comparing each bit in byte with 1, by shifting byte to the right
    for (i = 0; i < BYTE_SIZE; i++) {
        if (!(byte & 1)) {
            zero_bits++;
        }
        byte >>= 1;
    }
    return zero_bits;
}

/******************************************************************************/
unsigned int StandSeatsAvailable(Stand stand) {
    int i;
    unsigned int available_seats = 0;
    unsigned int num_bytes = get_num_seats_in_bytes(stand->num_seats);
    for (i = 0; i < num_bytes; i++) {
        available_seats += count_zero_bits(stand->seats[i]);
    }
    return available_seats;
}

/******************************************************************************/
unsigned int get_seat_index(Seat seat) {
    // get the index of the seat in the byte
    unsigned int seat_index = seat % BYTE_SIZE;
    return seat_index;
}

/******************************************************************************/
unsigned int get_seat_byte_index(Seat seat) {
    // get the index of the byte of the seat
    unsigned int seat_byte_index = seat / BYTE_SIZE;
    return seat_byte_index;
}

/******************************************************************************/
unsigned int StandSeatTaken(Stand stand, Seat seat) {
    bool is_invalid_seat = seat > stand->num_seats - 1;
    if (is_invalid_seat) {
        return INVALID_SEAT;
    }
    unsigned int seat_index = get_seat_index(seat);
    unsigned int seat_byte_index = get_seat_byte_index(seat);
    // comparing byte with a mask which all his bits are zero except seat_index
    bool is_taken = stand->seats[seat_byte_index] & (1 << seat_index);
    if (is_taken) {
        return true;
    }
    return false;
}

/******************************************************************************/
bool StandSeatAvailable(Stand stand, Seat seat) {
    bool is_taken = StandSeatTaken(stand, seat);
    return is_taken ? false : true;
}

/******************************************************************************/
bool StandBuyTicket(Stand stand, Seat seat) {
    bool is_taken = StandSeatTaken(stand, seat);
    if (!is_taken) {
        unsigned int seat_index = get_seat_index(seat);
        unsigned int seat_byte_index = get_seat_byte_index(seat);
        // mark the seat as taken
        stand->seats[seat_byte_index] |= (1 << seat_index);
        return true;
    } else {
        return false;
    }
}

/******************************************************************************/
bool StandBuyNTickets(Stand stand, Seat seats[], unsigned int num_seats) {
    int i;
    for (i = 0; i < num_seats; i++) {
        bool is_available = StandBuyTicket(stand, seats[i]);
        if (!is_available) {
            return false;
        }
    }
    return true;
}

/******************************************************************************/
// my StandBuyNConsecutiveTickets function

//Seat StandBuyNConsecutiveTickets(Stand stand, unsigned int num_seats) {
//    int consecutive_seats = 0;
//    int first_seat = -1;
//    Seat seat;
//    unsigned int total_seats = stand->num_seats;
//
//    for (seat = 0; seat < total_seats; seat++) {
//        bool is_taken = StandSeatTaken(stand, seat);
//        if (!is_taken) {
//            if (consecutive_seats == 0) {
//                first_seat = (int) seat;
//            }
//            consecutive_seats++;
//            if (consecutive_seats == num_seats) {
//                // buy seats
//                for (int i = 0; i < num_seats; ++i) {
//                    StandBuyTicket(stand, first_seat + i);
//                }
//                return first_seat;
//            }
//        } else {
//            consecutive_seats = 0;
//            first_seat = -1;
//        }
//    }
//    return NO_SEAT;
//}

/******************************************************************************/
/*
   the following two functions are as you demanded to implement
   StandBuyNConsecutiveTickets in the assignment
*/

Seat count_consecutive_available_seats(Stand stand, unsigned int num_seats) {
    int i, j;
    Seat first_seat = -1;
    unsigned int available_seats = 0;
    unsigned int num_bytes = get_num_seats_in_bytes(stand->num_seats);

    for (i = 0; i < num_bytes; i++) {
        for (j = 0; j < BYTE_SIZE; j++) {
            Seat curr_seat = i * BYTE_SIZE + j;
            bool is_available = StandSeatAvailable(stand, curr_seat);
            if (is_available) {
                if (available_seats == 0) {
                    first_seat = curr_seat;
                }
                available_seats++;
            } else {
                if (available_seats == num_seats) {
                    return first_seat;
                }
                // reset the counting
                first_seat = -1;
                available_seats = 0;
            }
        }
    }
    return first_seat;
}

/******************************************************************************/
Seat StandBuyNConsecutiveTickets(Stand stand, unsigned int num_seats) {
    int first_seat, last_seat;
    // if the number of seats is greater than stand capacity,
    if (num_seats > stand->num_seats) {
        return NO_SEAT;
    }

    first_seat = (int) count_consecutive_available_seats(stand, num_seats);
    if (first_seat == NO_SEAT) {
        return NO_SEAT;
    }

    unsigned int first_seat_index = get_seat_index(first_seat);
    unsigned int first_seat_byte_index = get_seat_byte_index(first_seat);
    // if all seats are in one byte
    if (num_seats <= BYTE_SIZE) {
        // mark num_seats seats as taken starting from the first_seat
        stand->seats[first_seat_byte_index] |= (((1 << num_seats) - 1) << first_seat_index);
        return first_seat;
    }
    /*
     * if seats are not in one byte:
     * We will find the byte where the first seat is located and turn on the
     * [first seat - end of byte] bits
     * then find the byte where the last seat is located and turn on the
     * [0 - last seat] bits
     * then if there is bytes between the first and last seats bytes,
     * we will turn them on
     * */
    // turn on the [first seat - end of byte] bits
    stand->seats[first_seat_byte_index] |= ((1 << (BYTE_SIZE - first_seat_index)) - 1) << first_seat_index;

    // turn on the [0 - last seat] bits
    last_seat = (int) (first_seat + num_seats - 1);
    unsigned int last_seat_index = get_seat_index(last_seat);
    unsigned int last_seat_byte_index = get_seat_byte_index(last_seat);
    stand->seats[last_seat_byte_index] |= (1 << (last_seat_index + 1)) - 1;

    // if there are bytes between the first and last seats, we will turn them on
    unsigned int bytes_between = last_seat_byte_index - first_seat_byte_index;
    if (bytes_between >= 2) {
        unsigned int i;
        for (i = first_seat_byte_index + 1; i < last_seat_byte_index; i++) {
            stand->seats[i] |= ((1 << BYTE_SIZE) - 1);
        }
    }
    return first_seat;
}