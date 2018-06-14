


/*

//if pilot sends no data. Goto stable

^scrath this, always send data, if it is 0, then it is 0.

define all pilot intentions as numbers with 0 being stable negative left and down, positve up and right

define current position in the same way

total thrust should only be dependant on throttle

if pilot send data, go to fraction of stick, with maximum being defined

*/




	void horizontal_controll(int pilot_intention_horizontal, int current_position_horizontal){


		if (pilot_intention_horizontal < current_position_horizontal){

			// quad is too far right

			// increase power to right front and right back

			// decrease power to left front and left back by the same amount


			// this should be done proportianally to how far the quad has banked and how hard the pilot steers

		}

		else if (pilot_intention_horizontal > current_position_horizontal){

			// quad is tilted left

			// increase power to left front and left back

			// decrease power to right front and right back by the same amount


			// this should be done proportianally to how far the quad has banked and how hard the pilot steers

		}

	}




	void vertical_controll(int pilot_intention_vertical, int current_position_vertical){

		if (pilot_intention_vertical < current_position_vertical){

			// quad is tilted down

			// increase power to right front and left front

			// decrease power to left back and right back by the same amount


			// this should be done proportianally to how far the quad has pitched and how hard the pilot steers

		}

		else if (pilot_intention_vertical > current_position_vertical){

			// quad is tilted up

			// increase power to right back and left back

			// decrease power to right front and left front by the same amount


			// this should be done proportianally to how far the quad has pitched and how hard the pilot steers

		}
	}


	void yaw_controll(int pilot_intention_yaw){

		if (pilot_intention_yaw > 0){

			// quad needs to rotate clockwise
 
			// increase power to left front and right back

			// decrease power to right front and left back

			// this should be done proportianally how hard the pilot steers


		}


		if (pilot_intention_yaw < 0){

			// quad needs to rotate counterclockwise

			// increase power to right front and left back

			// decrease power to left front and right back

			// this should be done proportianally how hard the pilot steers


		}
	}
