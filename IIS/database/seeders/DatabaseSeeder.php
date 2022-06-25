<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\Hash;
use \App\Models\User;
use \App\Models\Photo;
use \App\Models\Auction;
use \App\Models\Offer;

class DatabaseSeeder extends Seeder
{
    /**
     * Seed the application's database.
     *
     * @return void
     */
    public function run()
    {
        $admin=array(
            'name' => 'admin',
            'username' => 'Admin',
            'licitator' => 1,
            'email' => 'admin@admin.cz',
            'password' => Hash::make('admin1234'),
        );
        User::create($admin);
        $licitator=array(
            'name' => 'licitator',
            'username' => 'licitator',
            'licitator' => 1,
            'email' => 'licitator@licitator.cz',
            'password' => Hash::make('licitator1234'),
        );
        User::create($licitator);
        $user=array(
            'name' => 'user',
            'username' => 'user',
            'licitator' => 0,
            'email' => 'user@user.cz',
            'password' => Hash::make('user1234'),
        );
        User::create($user);
        User::factory(10)->create();
        Auction::factory(50)->create();
        if(!is_dir('public/img/'))
        {
            mkdir('public/img/');
        }
        foreach (Auction::all() as $auction)
        {
            if (!is_dir('public/img/'.$auction->id))
            {
                mkdir('public/img/'.$auction->id);
            }
        }
        Photo::factory(100)->create();
        //Offer::factory(500)->create();
    }
}
