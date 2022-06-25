<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Auction extends Model
{
    use HasFactory;
    protected $guarded=[];

    public function user()
    {
        return $this->belongsTo(User::class, 'seller_id', 'id');
    }
    public function licitator()
    {
        return $this->belongsTo(User::class, 'licitator_id', 'id');
    }
    public function photo()
    {
        return $this->hasMany(Photo::class);
    }
    public function offer()
    {
        return $this->hasMany(Offer::class, 'auction_id', 'id');
    }

    public function scopeFilter($query, array $filters)
    {
        $query->when(isset($filters['search']) ? $filters['search'] : false, function($query, $search){
            $query->where(fn($query) =>
                $query
                    ->whereExists(fn($query) =>
                        $query->from('users')
                            ->whereColumn('users.id', 'auctions.seller_id')
                            ->where('users.username', 'like', '%' . $search . '%'))
                            ->orWhere('name', 'like', '%' . $search . '%')
            );
        });

        $query->when(isset($filters['type']) ? $filters['type'] : false, function($query, $type){
            $query
                ->where('type', $type);
        });

        if(isset($filters['rule'])){
            $rule = $filters['rule'] == "open";
            $query->where('is_open', $rule);
        }
    }
}
