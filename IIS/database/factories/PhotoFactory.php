<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\Auction;

class PhotoFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array
     */
    public function definition()
    {
        return [
            'auction_id' => $lol=Auction::all()->random()->id,
            'filename' => 'img'.'/'.$lol.'/'.$this->faker->image('public/img'.'/'.$lol,640,480, null, false),
        ];
    }
}
