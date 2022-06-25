<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\User;
use App\Models\Auction;

class OfferFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array
     */
    public function definition()
    {
        return [
            'user_id' => User::all()->random()->id,
            'auction_id' => Auction::all()->random()->id,
            'value' => $this->faker->numberBetween(0, 1000000),
            'approved' => $this->faker->boolean($ChanceOfGettingTrue=10),
        ];
    }
}
