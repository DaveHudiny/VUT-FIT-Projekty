<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\User;

class AuctionFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array
     */
    public function definition()
    {
        return [
            'seller_id' => User::all()->random()->id,
            'licitator_id' => 0,
            'approved' => $this->faker->boolean($ChanceOfGettingTrue=0),
            'name' => $this->faker->sentence(),
            'description' => $this->faker->paragraph(),
            'type' => $this->faker->randomElement(['offer', 'demand']),
            'starting_price' => $this->faker->numberBetween(0, 10000),
            'minimum_wager' => $this->faker->numberBetween(0, 100),
            'requirements' => $this->faker->paragraph(),
            'date' => now()->addDays($this->faker->numberBetween(0, 30)),
            'is_open' => $this->faker->boolean($ChanceOfGettingTrue=50),
            'ended'=>$this->faker->boolean($ChanceOfGettingTrue=0),
            'confirmed'=>$this->faker->boolean($ChanceOfGettingTrue=0),
            'winner_id' => 0, 
        ];
    }
}
