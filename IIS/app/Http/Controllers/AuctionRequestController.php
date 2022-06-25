<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use \App\Models\Auction;
use \App\Models\Photo;
use \App\Models\Offer;

use App\Providers\RouteServiceProvider;

use Illuminate\Support\Facades\Route;


class AuctionRequestController extends Controller
{
    //

    public function requestBid(Request $request)
    {
        $request->validate([
            'userID'=>'required',
            'auctionID'=>'required',
            'type'=>'required',
            'bid'=>'required',
        ]
        );
        $maxOffer=Offer::where('auction_id', '=', $request['auctionID'])->where('approved', '=', true);
        $maxOffer=$maxOffer->where('value', '=', $maxOffer->max('value'))->get();
        $minOffer=Offer::where('auction_id', '=', $request['auctionID'])->where('approved', '=', true);
        $minOffer=$minOffer->where('value', '=', $minOffer->min('value'))->get();
        $auction=Auction::where('id', '=', $request['auctionID'])->get();
        $auction=$auction->first();
        //ddd($auction);
        $maxOffer=$maxOffer->first();
        $minOffer=$minOffer->first();
        if (!$maxOffer || $auction['is_open']==false)
        {
            $maxOffer=new Offer;
            //ddd($maxOffer);
            $maxOffer->value=$auction->starting_price;
        }
        if (!$minOffer || $auction['is_open']==false)
        {
            $minOffer=new Offer;
            $minOffer->value=$auction->starting_price;
        }
        if ($request['type']=='offer')
        {

            if ($request['bid']<$maxOffer->value+$auction->minimum_wager)
                {
                    return back()->withInput($request->only('bid'))
                    ->withErrors(['bid' => "Your bid is too low."]);;
                }
        }
        if ($request['type']=='demand')
        {
            if ($request['bid']>$minOffer->value-$auction->minimum_wager)
                {
                    return back()->withInput($request->only('bid'))
                    ->withErrors(['bid' => "Your bid is not low enough"]);;
                }
        }
        
        $attributes['value']=$request['bid'];
        if ($request['action']=='First bid')
        {
            $attributes['user_id']=$request['userID'];
            $attributes['auction_id']=$request['auctionID'];
            $attributes['approved']=false;
            Offer::create($attributes);
        }
        else
        {
            $users_bid=Offer::where('user_id', '=', $request['userID'])
                            ->where('auction_id', '=', $request['auctionID'])
                            ->where('approved', '=', true);
            if (!$users_bid)
            {
                return back()->withInput($request->only('bid'))
                ->withErrors(['bid' => "Your are not registered to this auction yet"]);;
            }
            $users_bid_value=$users_bid->get()->first();
            if ($request['type']=='offer' && $users_bid_value->value==$maxOffer->value)
            {
                return back()->withInput($request->only('bid'))
                ->withErrors(['bid' => "You're outbidding yourself"]);;
            }
            if ($request['type']=='demand' && $users_bid_value->value==$minOffer->value)
            {
                return back()->withInput($request->only('bid'))
                ->withErrors(['bid' => "You're outbidding yourself"]);;
            }
            if ($auction['is_open']==false)
            {
                return back()->withInput($request->only('bid'))
                ->withErrors(['bid' => "You can't bid twice on a closed auction"]);;
            }
            $users_bid->update(['value' => $attributes['value']]);
        }
        return view('auction', ['auction' => Auction::findOrFail($request['auctionID'])]);
    }
    public function request(Request $request)
    {
        //prida aukci podle requestu
        //aukci musi schvalit licitator
        $request->validate([
            'id'=>'required',
            'name'=>'required',
            'desc'=>'required',
            'type'=>'required',
            'price'=>'required',
            'minimum_wager'=>'required',
            'rules'=>'required',
            'requirements'=>'',
            'date'=>'required',
            'myphoto'=>'',
        ]
        );
        $attributes['seller_id']=$request['id'];
        $attributes['licitator_id']=0;
        $attributes['approved']=0;
        $attributes['name']=$request['name'];
        $attributes['description']=$request['desc'];
        $attributes['type']=$request['type'];
        $attributes['starting_price']=$request['price'];
        $attributes['confirmed']=false;
        $attributes['winner_id']=0;
        if ($request['minimum_wager'])
        {
            $attributes['minimum_wager']=$request['minimum_wager'];
        }
        else $attributes['minimum_wager']=0;
        $attributes['requirements']=$request['requirements'];
        $attributes['date']=$request['date'];
        if ($request['rules']=='open')
        {
            $attributes['is_open']=true;
        }
        else $attributes['is_open']=false;
        $attributes['ended']=false;
        $entity=Auction::create($attributes);
        if ($request['myphoto'])
        {
            $attr_photo['auction_id']=$entity['id'];
            $attr_photo['filename']='img/'.$entity['id'].'/'.$request['myphoto']->getClientOriginalName();
            Photo::create($attr_photo);
            $file=$request['myphoto'];
            $destinationPath='img/'.$entity['id'];
            //ddd($file);
            $file->move($destinationPath, $attr_photo['filename']);
        }
        return redirect(RouteServiceProvider::HOME);
    }
    public function edit(Request $request)
    {

    }
}
