<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use \App\Models\User;

class UserChangesController extends Controller
{
    private function delete(Request $request)
    {
        if($request->username == "Admin")
        {
            return;
        }
        User::where('username', '=', $request->username)->delete();
    }

    private function promote(Request $request)
    {
        User::where('username', '=', $request->username)->update(array('licitator' => true));
    }
    private function degrade(Request $request)
    {
        User::where('username', '=', $request->username)->update(array('licitator' => false));
    }

    public function edit(Request $request)
    {
        if($request->action=="Delete")
        {
            $this->delete($request);
        }
        else if($request->action=="Promote")
        {
            $this->promote($request);
        }
        else
        {
            $this->degrade($request);
        }
        return view('users', [ 'users' => User::all()]);
    }
    //
}


