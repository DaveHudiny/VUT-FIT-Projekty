<?php

namespace App\Models;

use Illuminate\Contracts\Auth\MustVerifyEmail;
use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Foundation\Auth\User as Authenticatable;
use Illuminate\Notifications\Notifiable;
use Laravel\Sanctum\HasApiTokens;

class User extends Authenticatable
{
    use HasApiTokens, HasFactory, Notifiable;

    /**
     * The attributes that are mass assignable.
     *
     * @var string[]
     */
    protected $guarded=[];

    public function selling()
    {
        return $this->hasMany(Auction::class, 'seller_id', 'id');
    }
    public function licitating()
    {
        return $this->hasMany(Auction::class, 'licitator_id', 'id');
    }
    public function offer()
    {
        return $this->hasMany(Offer::class, 'user_id', 'id');
    }
}
